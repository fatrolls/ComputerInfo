#ifndef WINSERVICE_H
#define WINSERVICE_H

// ����win�ķ���
//
//

#pragma once

#include "..\\..\\Server\ServerParam.h"

#ifdef WINSERVICE_DLL
#define WINSERVICEAPI _declspec(dllexport)
#else
#define WINSERVICEAPI _declspec(dllimport)
#endif

#ifdef  __cplusplus
extern  "C"{
#endif

	class CWinService : public CServerParam
	{
	public:

		CWinService();
		~CWinService();

	protected:

	private:
	};

#ifdef __cplusplus
}
#endif

#endif
