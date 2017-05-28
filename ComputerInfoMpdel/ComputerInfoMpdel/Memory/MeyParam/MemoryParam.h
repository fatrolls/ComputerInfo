#ifndef MEMORYPARAM_H
#define MEMORYPARAM_H

#pragma once
//#include <windows.h>
#include <Windows.h>
#include "..\\..\\Define.h"


#ifdef MEMORYPARAM_DLL
#define MEMORYPARAMAPI _declspec(dllexport)
#else
#define MEMORYPARAMAPI _declspec(dllimport)
#endif

#ifdef  __cplusplus
extern  "C"{
#endif

	class CMomeryParam
	{
	public:

		CMomeryParam();
		~CMomeryParam();

		CMomeryParam(MEMORYSTATUS memorystatus);

	public:

		//typedef struct _MEMORYSTATUS {
		//	DWORD  dwLength;
		//	DWORD  dwMemoryLoad;
		//	SIZE_T dwTotalPhys;
		//	SIZE_T dwAvailPhys;
		//	SIZE_T dwTotalPageFile;
		//	SIZE_T dwAvailPageFile;
		//	SIZE_T dwTotalVirtual;
		//	SIZE_T dwAvailVirtual;
		//} MEMORYSTATUS, *LPMEMORYSTATUS;

	

		// 物理内存使用率
		DWORD  GetMemoryLoad();
		// 物理内存总数
		DWORD  GetTotalPhys();
		// 物理内存可用数
		DWORD GetAvailPhys();
		// 页文件总数
		DWORD TotalPageFile();
		// 页文件用数
		DWORD GetAvailPageFile();
		// 虚拟内存总数
		DWORD GetTotalVirtual();
		// 虚拟内存可用数
		DWORD GetAvailVirtual();


		//  清理内存
		DWORD ClearMemory();


	public:

		void Set(){};
		void Get(){};

	protected:

		// 内存状态
		MEMORYSTATUS MyGlobalMemoryStatus();

	private:

		MEMORYSTATUS m_MemoryStatus;  //获得物理内存参数

	};


#ifdef  __cplusplus
}
#endif



#endif