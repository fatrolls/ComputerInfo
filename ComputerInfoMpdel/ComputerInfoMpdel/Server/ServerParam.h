#ifndef SERVERPARAM_H
#define SERVERPARAM_H

#pragma once

#include <Windows.h>
#include<stdio.h>

#ifdef MEMORYPARAM_DLL
#define MEMORYPARAMAPI _declspec(dllexport)
#else
#define MEMORYPARAMAPI _declspec(dllimport)
#endif

#ifdef  __cplusplus
extern  "C"{
#endif

	class CServerParam{
	public:

		CServerParam();
		~CServerParam();

	public:

		void ShowServiceList(DWORD dwServiceType);
		void OnBtnban(char szServiceName[MAXBYTE]);
		void OnBtnstart(char szServiceName[MAXBYTE]);

	protected:

	private:
	};

#ifdef  __cplusplus
}
#endif
#endif 