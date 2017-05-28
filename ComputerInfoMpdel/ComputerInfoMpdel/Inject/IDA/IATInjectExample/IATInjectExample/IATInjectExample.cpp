// FatherProc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <strsafe.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

using namespace std;

#define NTSTATUS long

typedef struct _PEB {
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2;
	DWORD dwMutant;
	DWORD dwImageBase;
	BYTE Reserved3[220];
	PVOID Reserved4[59];
	ULONG SessionId;
} PEB, *PPEB;

typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation = 0,
	ProcessWow64Information = 26
} PROCESSINFOCLASS;

typedef NTSTATUS (WINAPI *NtQueryInformationProcessPtr)(
	HANDLE           ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID            ProcessInformation,
	ULONG            ProcessInformationLength,
	PULONG           ReturnLength);

// 导入表定义
typedef struct tag_IATTableContent
{
	IMAGE_THUNK_DATA OriginalThunk[2];
	IMAGE_THUNK_DATA FirstThunk[2];
	IMAGE_IMPORT_BY_NAME ImportByName;
	CHAR szFuncName[64];
	CHAR szDllName[MAX_PATH];
}IATTableContent, *LPIATTableContent;

typedef struct _PROCESS_BASIC_INFORMATION
{
	DWORD ExitStatus;		// 接收进程终止状态
	DWORD PebBaseAddress;	// 接收进程环境块地址
	DWORD AffinityMask;		// 接收进程关联掩码
	DWORD BasePriority;		// 接收进程的优先级类
	ULONG UniqueProcessId;	// 接收进程ID
	ULONG InheritedFromUniqueProcessId; //接收父进程ID
} PROCESS_BASIC_INFORMATION;

//////////////////////////////////////////////////////////////////////////

void PrintInfoW(LPCWSTR fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	WCHAR wbuf[512];
	StringCchVPrintfW(wbuf, 512, fmt, args);
	OutputDebugStringW(wbuf);

	va_end(args);
}

void PrintInfoA(LPCSTR fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	CHAR buf[512];
	StringCchVPrintfA(buf, 512, fmt, args);
	OutputDebugStringA(buf);

	va_end(args);
}

BOOL ModifyIATTableToLoadDll(DWORD dwProcessId)
{
	if ( dwProcessId <= NULL)
		return FALSE;
	
	LPVOID lpBaseAddress = NULL;

	HANDLE hProcess = OpenProcess( PROCESS_VM_WRITE|PROCESS_VM_READ|PROCESS_VM_OPERATION|PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
	if ( NULL == hProcess)
		return FALSE;

	// Get Exe Base Address	
	NtQueryInformationProcessPtr My_NtQueryInformationProcess = (NtQueryInformationProcessPtr)GetProcAddress( GetModuleHandleW(L"ntdll.dll"), "NtQueryInformationProcess");
	if ( NULL == My_NtQueryInformationProcess)	
		return FALSE;
	
	PEB _PEBStruct = {};
	DWORD dwRetSize = 0;
	PROCESS_BASIC_INFORMATION BasicInfo = {};
	HRESULT hRet = My_NtQueryInformationProcess( hProcess, ProcessBasicInformation, &BasicInfo, sizeof(BasicInfo), &dwRetSize);
	if ( hRet < 0)
		return FALSE;

	if( !ReadProcessMemory(hProcess, (LPVOID)BasicInfo.PebBaseAddress, &_PEBStruct, sizeof(PEB), &dwRetSize) || dwRetSize != sizeof(PEB))
		return FALSE;
	
	lpBaseAddress = (LPVOID)_PEBStruct.dwImageBase;

	BYTE* lpBuffer = new BYTE[1024*4];
	if ( NULL == lpBuffer)
		return FALSE;
	memset(lpBuffer, 0, 1024*4);
	if ( !ReadProcessMemory( hProcess, lpBaseAddress, lpBuffer, 1024*4, &dwRetSize) || dwRetSize != 1024*4)
		return FALSE;

	PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)lpBuffer;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)DosHeader + DosHeader->e_lfanew);
	if ( NULL == pNTHeader)
		return FALSE;
	PIMAGE_DATA_DIRECTORY pDataDirectory = &(pNTHeader->OptionalHeader.DataDirectory[0]);
	if ( NULL == pDataDirectory)
		return FALSE;
	
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)(pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress + (LONG_PTR)lpBaseAddress);
	DWORD dwIATDESCSize = pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size + sizeof(IMAGE_IMPORT_DESCRIPTOR) * 2;
	BYTE * lpIATDescriptor = new BYTE[dwIATDESCSize];
	memset( lpIATDescriptor, 0, dwIATDESCSize);
	if ( !ReadProcessMemory( hProcess, pImportDesc, lpIATDescriptor, dwIATDESCSize, &dwRetSize) || dwRetSize != dwIATDESCSize)
		return FALSE;


	PIMAGE_IMPORT_DESCRIPTOR pImportDescTemp = (PIMAGE_IMPORT_DESCRIPTOR)lpIATDescriptor;
	for( int i = 0; pImportDescTemp->FirstThunk != NULL; i++, pImportDescTemp++)
	{
		PrintInfoA( "Import Descriptor %d : %x\n", i, pImportDescTemp->Name);
	}
	memset( pImportDescTemp, 0, sizeof(IMAGE_IMPORT_DESCRIPTOR)*2);

	IATTableContent iatTableContent = {};
	iatTableContent.FirstThunk[0].u1.AddressOfData = offsetof( IATTableContent, ImportByName);
	iatTableContent.OriginalThunk[0].u1.AddressOfData = offsetof( IATTableContent, ImportByName);
	CHAR szDllPath[MAX_PATH*2] = {};
	GetModuleFileNameA(NULL, szDllPath, MAX_PATH*2);
	PathAppendA( szDllPath, "..\\Dll.dll");
	StringCchCopyA( iatTableContent.szDllName, MAX_PATH, szDllPath);
	PrintInfoA("LoadDll: %s\n", szDllPath);
	StringCchCopyA( (char*)(iatTableContent.ImportByName.Name), 64, "HelloShine");
	
	LPVOID lpAllocAddr = (LPVOID)(( ((LONG_PTR)lpBaseAddress + pNTHeader->OptionalHeader.SizeOfImage) / 4096 + 1) * 4096);
	DWORD dwAllocSize = ( (dwIATDESCSize + sizeof(IATTableContent)) / 4096 + 1)*4096;	
	LPVOID lpNewImport = VirtualAllocEx( hProcess, NULL, dwAllocSize, MEM_COMMIT|MEM_TOP_DOWN, PAGE_READWRITE);
	if ( NULL == lpNewImport)
	{
		if ( NULL != lpBuffer)
		{
			delete [] lpBuffer;
		}
		if ( NULL != lpIATDescriptor)
		{
			delete [] lpIATDescriptor;
		}
		CloseHandle(hProcess);
		return FALSE;
	}

	pImportDescTemp->FirstThunk			= (LONG_PTR)lpNewImport + dwIATDESCSize + offsetof( IATTableContent, FirstThunk) - (LONG_PTR)lpBaseAddress;
	pImportDescTemp->OriginalFirstThunk = (LONG_PTR)lpNewImport + dwIATDESCSize + offsetof( IATTableContent, OriginalThunk) - (LONG_PTR)lpBaseAddress;
	pImportDescTemp->Name				= (LONG_PTR)lpNewImport + dwIATDESCSize + offsetof( IATTableContent, szDllName) - (LONG_PTR)lpBaseAddress;
	iatTableContent.FirstThunk[0].u1.AddressOfData		= (LONG_PTR)lpNewImport + dwIATDESCSize + offsetof( IATTableContent, ImportByName) - (LONG_PTR)lpBaseAddress;
	iatTableContent.OriginalThunk[0].u1.AddressOfData	= (LONG_PTR)lpNewImport + dwIATDESCSize + offsetof( IATTableContent, ImportByName) - (LONG_PTR)lpBaseAddress;
	
	WriteProcessMemory( hProcess, lpNewImport, lpIATDescriptor, dwIATDESCSize, &dwRetSize);
	WriteProcessMemory( hProcess, (BYTE*)lpNewImport+dwRetSize, &iatTableContent, sizeof(IATTableContent), &dwRetSize);

	//这里修改导入表的入口点
	DWORD dwOldProtect = 0;
	IMAGE_DATA_DIRECTORY DataDir = {};
	DataDir.VirtualAddress = (LONG_PTR)lpNewImport - (LONG_PTR)lpBaseAddress;
	DataDir.Size = dwIATDESCSize - sizeof(IMAGE_IMPORT_DESCRIPTOR);

	DWORD dwOffset = (BYTE*)pNTHeader - lpBuffer + offsetof(IMAGE_NT_HEADERS, OptionalHeader) + offsetof(IMAGE_OPTIONAL_HEADER32, DataDirectory) + sizeof(IMAGE_DATA_DIRECTORY)*IMAGE_DIRECTORY_ENTRY_IMPORT;

	VirtualProtectEx( hProcess, (LPVOID)(dwOffset + (LONG_PTR)lpBaseAddress), 8, PAGE_READWRITE, &dwOldProtect);
	WriteProcessMemory( hProcess, (LPVOID)((LONG_PTR)lpBaseAddress + dwOffset), &DataDir, sizeof(IMAGE_DATA_DIRECTORY), &dwRetSize);	
	VirtualProtectEx( hProcess, (LPVOID)(dwOffset + (LONG_PTR)lpBaseAddress), 8, dwOldProtect, &dwOldProtect);

	CloseHandle(hProcess);
	if ( NULL != lpBuffer)
	{
		delete [] lpBuffer;
	}
	if ( NULL != lpIATDescriptor)
	{
		delete [] lpIATDescriptor;
	}	

	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	WCHAR szExePath[MAX_PATH] = {};
	GetModuleFileNameW( NULL, szExePath, MAX_PATH);
	PathAppendW( szExePath, L"..\\Test.exe");
	
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	BOOL bRet = CreateProcessW(
				szExePath,
				NULL,    
				NULL,
				NULL,
				FALSE,
				CREATE_SUSPENDED,
				NULL,
				NULL,
				&si,
				&pi);


	// Modify IAT table
	ModifyIATTableToLoadDll(pi.dwProcessId);

	ResumeThread(pi.hThread);
	if ( pi.hProcess )
		CloseHandle(pi.hProcess);
	if (pi.hThread)
		CloseHandle(pi.hThread);
	
	return 0;
}

