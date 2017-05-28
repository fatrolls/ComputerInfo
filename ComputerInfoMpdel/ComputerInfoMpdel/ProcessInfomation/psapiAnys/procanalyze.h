#pragma once
//#include <windows.h>
#include <Windows.h>
#include <concrt.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <Strsafe.h>

#include "..\\Psapi\\Psapi.h"

#include<iostream>
using namespace  std;

#ifndef PROCANALYZE_H
#define PROCANALYZE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4201)   // unnamed struct
#pragma warning(disable:4214)   // bit fields other than int

	typedef union _PSAPI_WORKING_SET_BLOCK {
		ULONG_PTR Flags;
		struct {
			ULONG_PTR Protection : 5;
			ULONG_PTR ShareCount : 3;
			ULONG_PTR Shared : 1;
			ULONG_PTR Reserved : 3;
#if defined(_WIN64)
			ULONG_PTR VirtualPage : 52;
#else
			ULONG_PTR VirtualPage : 20;
#endif
		};
	} PSAPI_WORKING_SET_BLOCK, *PPSAPI_WORKING_SET_BLOCK;

	typedef struct _PSAPI_WORKING_SET_INFORMATION {
		ULONG_PTR NumberOfEntries;
		PSAPI_WORKING_SET_BLOCK WorkingSetInfo[1];
	} PSAPI_WORKING_SET_INFORMATION, *PPSAPI_WORKING_SET_INFORMATION;

	typedef union _PSAPI_WORKING_SET_EX_BLOCK {
		ULONG_PTR Flags;
		union {
			struct {
				ULONG_PTR Valid : 1;
				ULONG_PTR ShareCount : 3;
				ULONG_PTR Win32Protection : 11;
				ULONG_PTR Shared : 1;
				ULONG_PTR Node : 6;
				ULONG_PTR Locked : 1;
				ULONG_PTR LargePage : 1;
				ULONG_PTR Reserved : 7;
				ULONG_PTR Bad : 1;

#if defined(_WIN64)
				ULONG_PTR ReservedUlong : 32;
#endif
			};
			struct {
				ULONG_PTR Valid : 1;            // Valid = 0 in this format.
				ULONG_PTR Reserved0 : 14;
				ULONG_PTR Shared : 1;
				ULONG_PTR Reserved1 : 15;
				ULONG_PTR Bad : 1;

#if defined(_WIN64)
				ULONG_PTR ReservedUlong : 32;
#endif
			} Invalid;
		};
	} PSAPI_WORKING_SET_EX_BLOCK, *PPSAPI_WORKING_SET_EX_BLOCK;

	typedef struct _PSAPI_WORKING_SET_EX_INFORMATION {
		PVOID VirtualAddress;
		PSAPI_WORKING_SET_EX_BLOCK VirtualAttributes;
	} PSAPI_WORKING_SET_EX_INFORMATION, *PPSAPI_WORKING_SET_EX_INFORMATION;

	class ProcAnalyze
	{
	private:
		int EnablePrivilege(LPTSTR ptszPrivilegeName, BOOL bEnable = TRUE);		//
		int GetOSVer();													 //
	public:
		ProcAnalyze();
		~ProcAnalyze();

		//process
		int ListProcess();				  //
		int GetProcFullPath(DWORD dwPID, TCHAR *tszPath);	 //
		int KillProcess(DWORD dwPID);			  //
		//thread
		int ListThread(DWORD dwPID = 0);			  //
		int GetProcessName(DWORD dwPID, TCHAR *tszName);	   //
		//heap
		int ListHeap(DWORD dwPID = 0);
		//int ListMemory( DWORD dwPID = 0);
		int ListMemory(PPROCESS_MEMORY_COUNTERS ppsmem, DWORD dwPID = 0);
		//modules
		int ListModule(DWORD dwPID);
		//pages
		int ListPages(DWORD dwPID = 0);
	};

#ifdef __cplusplus
}
#endif

#endif
