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

	

		// �����ڴ�ʹ����
		DWORD  GetMemoryLoad();
		// �����ڴ�����
		DWORD  GetTotalPhys();
		// �����ڴ������
		DWORD GetAvailPhys();
		// ҳ�ļ�����
		DWORD TotalPageFile();
		// ҳ�ļ�����
		DWORD GetAvailPageFile();
		// �����ڴ�����
		DWORD GetTotalVirtual();
		// �����ڴ������
		DWORD GetAvailVirtual();


		//  �����ڴ�
		DWORD ClearMemory();


	public:

		void Set(){};
		void Get(){};

	protected:

		// �ڴ�״̬
		MEMORYSTATUS MyGlobalMemoryStatus();

	private:

		MEMORYSTATUS m_MemoryStatus;  //��������ڴ����

	};


#ifdef  __cplusplus
}
#endif



#endif