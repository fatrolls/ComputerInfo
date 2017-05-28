//进程是操作系统结构的基础；是一个正在执行的程序；计算机中正在运行的程序实例；可以分配给处理器并由处理器执行的一个实体；
//由单一顺序的执行显示，一个当前状态和一组相关的系统资源所描述的活动单元。
//对应用程序来说，进程就像一个大容器。在应用程序被运行后，就相当于将应用程序装进容器里了，
//你可以往容器里加其他东西(如:应用程序在运行时所需的变量数据、需要引用的DLL文件等)，当应用程序被运行两次时，
//容器里的东西并不会被倒掉，
//系统会找一个新的进程容器来容纳它。
//
//动态性：进程的实质是程序在多道程序系统中的一次执行过程，进程是动态产生，动态消亡的。
//并发性：任何进程都可以同其他进程一起并发执行
//独立性：进程是一个能独立运行的基本单位，同时也是系统分配资源和调度的独立单位；
//异步性：由于进程间的相互制约，使进程具有执行的间断性，即进程按各自独立的、不可预知的速度向前推进
//结构特征：进程由程序、数据和进程控制块三部分组成。 　　
//多个不同的进程可以包含相同的程序：一个程序在不同的数据集里就构成不同的进程，
//能得到不同的结果；但是执行过程中，程序不能发生改变
//一般Windows有若干个用户，有的是Administator, 有的是自定义的，进程都有用户创建的标识，
//我们如何将一个由自己的用户例如yincheng创建的进程，改成Administator的进程，以便获得更高的权限呢，

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

			if (!stricmp(CMutilpleCharasetToAscII::CStringToCChar(Pc.szExeFile), szExeName)) {   //返回explorer.exe进程的PID  
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
		NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi))   //以administrator用户身份执行程序,CREATE_NO_WINDOW,CREATE_NEW_CONSOLE,CREATE_DEFAULT_ERROR_MODE  
	{
		printf("CreateProcessAsUser sucessed!%d\n", GetLastError());
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return 0;
}

// test code
//impuser("regedit");
