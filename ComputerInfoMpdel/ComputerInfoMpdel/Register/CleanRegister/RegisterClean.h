/************************************************************************/
/*                  ����ע���                                            */
/*           1���̳���CRegisterOpear                                      */
/*           2������ǰ���б���                                             */
/*           3���������ʧ����ԭ                                          */
/*           4��������ʾ����켣������ɹ�									*/
/*			 5������·������												*/
/*                                                                      */
/*                                                                      */
/************************************************************************/

#pragma  once

#ifndef REGISTERCLEAN_H
#define REGISTERCLEAN_H

#pragma once
#include <windows.h>
#include "..\OpearRegister\OpearRegister.h"


#ifdef REGISTER_DLL
#define REGISTERCLEAN_API _declspec(dllexport)
#else
#define REGISTERCLEAN_API _declspec(dllimport)
#endif


class CRegisterClean :public CRegisterOpear
{
public:
	CRegisterClean(){};
	~CRegisterClean(){};
protected:
private:
};





#endif
