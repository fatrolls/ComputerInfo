//�����ǲ���ϵͳ�ṹ�Ļ�������һ������ִ�еĳ��򣻼�������������еĳ���ʵ�������Է�������������ɴ�����ִ�е�һ��ʵ�壻
//�ɵ�һ˳���ִ����ʾ��һ����ǰ״̬��һ����ص�ϵͳ��Դ�������Ļ��Ԫ��
//��Ӧ�ó�����˵�����̾���һ������������Ӧ�ó������к󣬾��൱�ڽ�Ӧ�ó���װ���������ˣ�
//����������������������(��:Ӧ�ó���������ʱ����ı������ݡ���Ҫ���õ�DLL�ļ���)����Ӧ�ó�����������ʱ��
//������Ķ��������ᱻ������
//ϵͳ����һ���µĽ�����������������
//
//��̬�ԣ����̵�ʵ���ǳ����ڶ������ϵͳ�е�һ��ִ�й��̣������Ƕ�̬��������̬�����ġ�
//�����ԣ��κν��̶�����ͬ��������һ�𲢷�ִ��
//�����ԣ�������һ���ܶ������еĻ�����λ��ͬʱҲ��ϵͳ������Դ�͵��ȵĶ�����λ��
//�첽�ԣ����ڽ��̼���໥��Լ��ʹ���̾���ִ�еļ���ԣ������̰����Զ����ġ�����Ԥ֪���ٶ���ǰ�ƽ�
//�ṹ�����������ɳ������ݺͽ��̿��ƿ���������ɡ� ����
//�����ͬ�Ľ��̿��԰�����ͬ�ĳ���һ�������ڲ�ͬ�����ݼ���͹��ɲ�ͬ�Ľ��̣�
//�ܵõ���ͬ�Ľ��������ִ�й����У������ܷ����ı�
//һ��Windows�����ɸ��û����е���Administator, �е����Զ���ģ����̶����û������ı�ʶ��
//������ν�һ�����Լ����û�����yincheng�����Ľ��̣��ĳ�Administator�Ľ��̣��Ա��ø��ߵ�Ȩ���أ�

#include "stdafx.h"  
#include "windows.h"  
#include <stdio.h>
#include <process.h>  
#include <Tlhelp32.h>  
#include <psapi.h>  

#include <tlhelp32.h>   
#pragma  comment (lib,"psapi")  
BOOL EnableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{

		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{

		CloseHandle(hToken);
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{

		return FALSE;
	}
	CloseHandle(hToken);
	return TRUE;
}


HANDLE GetProcessHandle(LPSTR szExeName)

{

	PROCESSENTRY32 Pc = { sizeof(PROCESSENTRY32) };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	if (Process32First(hSnapshot, &Pc)){

		do{

			if (!stricmp(CMutilpleCharasetToAscII::CStringToCChar(Pc.szExeFile), szExeName)) {   //����explorer.exe���̵�PID  
				printf("explorer's PID=%d\n", Pc.th32ProcessID);
				return OpenProcess(PROCESS_ALL_ACCESS, TRUE, Pc.th32ProcessID);

			}

		} while (Process32Next(hSnapshot, &Pc));

	}


	 
	return NULL;
}


// ProcessInfomation///process
// int ListProcess();  get id
int impuser(TCHAR cmdline[256])
{
	HANDLE hToken;
	HANDLE   hExp = GetProcessHandle("calc.EXE");
	if (hExp == NULL)
		return   FALSE;

	OpenProcessToken(hExp, TOKEN_ALL_ACCESS, &hToken);
	if (hToken == NULL)
		return   FALSE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = L"winsta0\\default";
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;

	TCHAR   szParameter[256] = L"/c ";
	lstrcat(szParameter, cmdline);
	printf("szParameter=%s\n", szParameter);

	char path[MAX_PATH];
	GetSystemWindowsDirectory((LPWSTR)path, MAX_PATH);
	lstrcat((LPWSTR)path, L"\\system32\\cmd.exe");

	if (CreateProcessAsUser(hToken, (LPCWSTR)(char *)path, szParameter, NULL,
		NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi))   //��administrator�û����ִ�г���,CREATE_NO_WINDOW,CREATE_NEW_CONSOLE,CREATE_DEFAULT_ERROR_MODE  
	{
		printf("CreateProcessAsUser sucessed!%d\n", GetLastError());
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return 0;
}

// test code
//impuser("regedit");
