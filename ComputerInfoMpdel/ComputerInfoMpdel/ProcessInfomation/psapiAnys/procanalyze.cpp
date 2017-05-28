#pragma  once

#include "stdafx.h"
#include "procanalyze.h"

#pragma warning(disable: 4996)

ProcAnalyze::ProcAnalyze()
{
}

ProcAnalyze::~ProcAnalyze()
{
}

int ProcAnalyze::EnablePrivilege(LPTSTR ptszPrivilegeName, BOOL bEnable/* =TRUE */)
{
	HANDLE hToken = NULL;
	LUID   luid = { 0 };
	TOKEN_PRIVILEGES tp = { 0 };

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)){
		return 0;
	}

	if (!LookupPrivilegeValue(NULL, ptszPrivilegeName, &luid)){
		//	Log(_T("LookupPrivilegeValue error:%d"), GetLastError());
		CloseHandle(hToken);
		return 0;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges->Luid = luid;
	if (bEnable)
		tp.Privileges->Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL)){
		//	Log(_T("AdjustTokenPrivileges error:%d"), GetLastError());
		CloseHandle(hToken);
		return 0;
	}

	CloseHandle(hToken);

	return 1;
}

int ProcAnalyze::GetOSVer()
{
	DWORD ver = GetVersion();
	if (LOWORD(ver) <= 5)
	{
		return 0;
	}
	else{
		return 1;
	}
}

int ProcAnalyze::ListProcess()
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = { sizeof(pe32) };

	if (hSnap != INVALID_HANDLE_VALUE) {
		if (Process32First(hSnap, &pe32)) {
			do{
				printf("进程名 %ls\n", pe32.szExeFile);
				printf("PID %d\n", pe32.th32ProcessID);
				printf("父进程PID %d\n", pe32.th32ParentProcessID);
				printf("线程数 %d\n", pe32.cntThreads);
				printf("线程基本优先级 %d\n", pe32.pcPriClassBase);
				printf("--------------------------------------\n");

				TCHAR tszExe[MAX_PATH] = { 0 };
				GetProcFullPath(pe32.th32ProcessID, tszExe);

				ListThread(pe32.th32ProcessID);
				ListHeap(pe32.th32ProcessID);

				PROCESS_MEMORY_COUNTERS psmem = { sizeof(psmem) };
				ListMemory(&psmem, pe32.th32ProcessID);
				ListModule(pe32.th32ProcessID);

				ListPages(pe32.th32ProcessID);
			} while (Process32Next(hSnap, &pe32));
		}
	}
	return 0;
}

int ProcAnalyze::GetProcFullPath(DWORD dwPID, TCHAR *tszPath)
{
	EnablePrivilege(SE_DEBUG_NAME);											   \

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	MODULEENTRY32 me32 = { sizeof(me32) };
	TCHAR tszExe[5] = { 0 };
	DWORD dwSize = MAX_PATH;
	HANDLE hProc = NULL;

	if (hSnap == INVALID_HANDLE_VALUE) {
		//		Log(_T("CreateToolhelp32Snapshot %d error:%d"), dwPID, GetLastError());

		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		if (!hProc) {
			//			Log(_T("OpenProcess %d error:%d"), dwPID, GetLastError());
			return 0;
		}
		if (GetOSVer() == 0){//xp
			if (!GetProcessImageFileName(hProc, tszPath, MAX_PATH)){
				//				Log(_T("GetProcessImageFileName %d error:%d"), dwPID, GetLastError());
				return 0;
			}
		}
		else{
			//if(!QueryFullProcessImageName(hProc, /*PROCESS_NAME_NATIVE*/0, tszPath, &dwSize)){
			//	Log(_T("QueryFullProcessImageName %d error:%d"), dwPID,  GetLastError());
			//	return 0;
			//}
		}

		return 0;
	}
	if (Module32First(hSnap, &me32)) {
		do{
			_tcsncpy_s(tszExe, me32.szModule + _tcslen(me32.szModule) - 4, 4);
			if (!_tcsicmp(tszExe, _T(".exe"))) {
				StringCbCopy(tszPath, MAX_PATH, me32.szExePath);
				printf("%ls\n", me32.szExePath);
				cout << "FullPath： " << me32.szExePath << endl;
				cout << "------------------------------" << endl;
				return 1;
			}
		} while (Module32Next(hSnap, &me32));
	}
	return 1;
}

int ProcAnalyze::KillProcess(DWORD dwPID)
{
	EnablePrivilege(SE_DEBUG_NAME);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hProc == NULL) {
		//		Log(_T("KillProcess->OpenProcess error:%d"), GetLastError());
		return 0;
	}

	if (!TerminateProcess(hProc, 0)){
		CloseHandle(hProc);
		return 0;
	}

	CloseHandle(hProc);

	return 1;
}

int ProcAnalyze::GetProcessName(DWORD dwPID, TCHAR *tszName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = { sizeof(pe32) };

	if (hSnap != INVALID_HANDLE_VALUE) {
		if (Process32First(hSnap, &pe32)) {
			do{
				if (pe32.th32ProcessID == dwPID) {
					StringCbCopy(tszName, MAX_PATH, pe32.szExeFile);
					return 1;
				}
			} while (Process32Next(hSnap, &pe32));
		}
	}
	return 0;
}

int ProcAnalyze::ListThread(DWORD dwPID)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 te32 = { sizeof(te32) };

	if (hSnap == INVALID_HANDLE_VALUE) {
		//		Log(_T("ListThread->CreateToolhelp32Snapshot error:%d"), GetLastError());
		return 0;
	}
	if (Thread32First(hSnap, &te32)) {
		do{
			/*if ((dwPID != 0) && (te32.th32OwnerProcessID != dwPID)) {
				continue;
				}*/
			printf("--------------ListThread-------------------\n");

			//	printf("进程名%ls\n", );
			printf("PID %d\n", dwPID);
			printf("TID %d\n", te32.th32ThreadID);
			printf("线程基本优先级 %d\n", te32.tpBasePri);
			printf("优先级变化 %d\n", te32.tpDeltaPri);
			printf("引用数 %d\n", te32.cntUsage);
			printf("dwFlags: %d\n", te32.dwFlags);
			printf("dwsize: %d\n", te32.dwSize);
			printf("th32OwnerProcessID: %d\n", te32.th32OwnerProcessID);
		} while (Thread32Next(hSnap, &te32));
	}
	return 1;
}

int ProcAnalyze::ListHeap(DWORD dwPID)
{
	if (dwPID <= 0) return 0;

	EnablePrivilege(SE_DEBUG_NAME);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPID);
	HEAPLIST32 hl32 = { sizeof(hl32) };
	HEAPENTRY32 he32 = { sizeof(he32) };

	if (hSnap == INVALID_HANDLE_VALUE){
		//		Log(_T("ListHeap->CreateToolhelp32Snapshot error:%d"), GetLastError());
		return 0;
	}

	if (Heap32ListFirst(hSnap, &hl32)) {
		do{
			he32.dwSize = sizeof(he32);
			if (Heap32First(&he32, hl32.th32ProcessID, hl32.th32HeapID)) {
				do{
					printf("堆句柄: %d \n", he32.hHandle);
					printf("起始地址: %d \n", he32.dwAddress);
					printf("大小:%d \n", he32.dwSize);
					printf("状态: %d\n", he32.dwFlags);
					printf("Lock数: %d\n", he32.dwLockCount);
					printf("PID: %d\n", he32.th32ProcessID);
					printf("--------------list heap--------------\n");
				} while (Heap32Next(&he32));
			}
		} while (Heap32ListNext(hSnap, &hl32));
	}
	return 1;
}

int ProcAnalyze::ListMemory(PPROCESS_MEMORY_COUNTERS psmem, DWORD dwPID /* = 0 */)
{
	EnablePrivilege(SE_DEBUG_NAME);

	if (dwPID <= 0) return 0;

	//PROCESS_MEMORY_COUNTERS psmem = {sizeof(psmem)};

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (hProc == 0) {
		//		Log(_T("ListMemory->OpenProcess error:%d"), GetLastError());
		return 0;
	}

	if (!GetProcessMemoryInfo(hProc, psmem, psmem->cb)){
		//		Log(_T("ListMemory->GetProcessMemoryInfo error:%d"), GetLastError());
		return 0;
	}

	printf("缺页中断次数: %d\n", psmem->PageFaultCount);
	printf("内存高峰: %d\n", psmem->PeakWorkingSetSize);
	printf("使用内存: %d\n", psmem->WorkingSetSize);
	printf("页面缓存池高峰: %d\n", psmem->QuotaPeakPagedPoolUsage);
	printf("页面缓存池: %d\n", psmem->QuotaPagedPoolUsage);
	printf("非分页缓存池高峰: %d\n", psmem->QuotaPeakNonPagedPoolUsage);
	printf("非分页缓存池: %d\n", psmem->QuotaNonPagedPoolUsage);
	printf("使用分页文件: %d\n", psmem->PagefileUsage);
	printf("使用分页文件高峰: %d\n", psmem->PeakPagefileUsage);
	printf("--------------------list memory-----------------------------\n");

	CloseHandle(hProc);

	return 1;
}

int ProcAnalyze::ListModule(DWORD dwPID)
{
	EnablePrivilege(SE_DEBUG_NAME);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	MODULEENTRY32 me32 = { sizeof(me32) };

	if (hSnap == INVALID_HANDLE_VALUE) {
		//		Log(_T("ListModule->CreateToolhelp32Snapshot error:%d"), GetLastError());
		return 0;
	}

	if (Module32First(hSnap, &me32)) {
		do{
			printf("名字: %ws\n", me32.szModule);
			printf("全局引用数: %d\n", me32.GlblcntUsage);
			printf("进程引用数: %d\n", me32.ProccntUsage);
			printf("基地址: 0x%08X\n", me32.modBaseAddr);
			printf("大小: %d\n", me32.modBaseSize);
			printf("句柄: 0x%08X\n", me32.hModule);
			printf("路径: %ws\n", me32.szExePath);

			printf("------------------list moudle-----------------\n");
		} while (Module32Next(hSnap, &me32));
	}

	return 1;
}

int ProcAnalyze::ListPages(DWORD dwPID /* = 0 */)
{
	if (dwPID <= 0) return 0;

	EnablePrivilege(SE_DEBUG_NAME);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	PPSAPI_WORKING_SET_INFORMATION pppswsi = new PSAPI_WORKING_SET_INFORMATION;

	memset(pppswsi, 0, sizeof(PSAPI_WORKING_SET_INFORMATION));
	if (!QueryWorkingSet(hProc, pppswsi, sizeof(PSAPI_WORKING_SET_INFORMATION))){
		if (ERROR_BAD_LENGTH == GetLastError()){
			int size = pppswsi->NumberOfEntries*sizeof(PSAPI_WORKING_SET_BLOCK)+sizeof(ULONG_PTR);
			pppswsi = (PPSAPI_WORKING_SET_INFORMATION)new BYTE[size];
			if (pppswsi != 0) {
				if (QueryWorkingSet(hProc, pppswsi, size)){
					//Log(_T("ListMemory->QueryWorkingSet error:%d"), GetLastError());
					// _func((PVOID)pppswsi);
					printf("------------------list peag------------------\n");

					for (DWORD i = 0; i < pppswsi->NumberOfEntries; i++) {
						printf("标志: %d\n", pppswsi->WorkingSetInfo[i].Flags);
						printf("保护: %d\n", pppswsi->WorkingSetInfo[i].Protection);
						printf("共享数: %d\n", pppswsi->WorkingSetInfo[i].ShareCount);
						printf("是否共享: %d\n", pppswsi->WorkingSetInfo[i].Shared);
						printf("虚拟地址: 0x%08X\n", pppswsi->WorkingSetInfo[i].VirtualPage);
					}

					delete[] pppswsi;
				}
				else{
					//					Log(_T("ListMemory->QueryWorkingSet error:%d"), GetLastError());
				}
			}
		}
	}

	CloseHandle(hProc);

	return 1;
}