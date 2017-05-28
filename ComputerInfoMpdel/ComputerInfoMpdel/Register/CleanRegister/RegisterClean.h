/************************************************************************/
/*                  清理注册表                                            */
/*           1、继承于CRegisterOpear                                      */
/*           2、清理前进行备份                                             */
/*           3、清理出现失败则还原                                          */
/*           4、清理显示清理轨迹，清理成功									*/
/*			 5、清理路径备份												*/
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
