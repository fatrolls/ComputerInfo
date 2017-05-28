/************************************************************************/
/*    注册表操作：                                                        */
/*        1、继承于Register                                              */
/*        2、功能点：查询，备份，删除，删除恢复，启动项管理，添加，               */
/*           注册表禁止，启动                                              */
/*        3、备份：1、软件启动备份；2、删除前备份                              */
/*        4、查询：													    */
/*				1、默认所有的注册表项查询                                   */
/*				2、指定查询注册表目录									    */
/*				3、查询的类型												*/
/*		  5、启动项管理：													*/
/*		  [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run]				*/
/*		  [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunOnce]（仅运行一次） */
/*		  [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunServices]		*/
/*                                                                      */
/************************************************************************/
// 启动项
//[HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run]
//[HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunServices]
//[HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce]
//[HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunServicesOnce]
//[HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run]
//[HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce]
//[HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunServices]

#pragma  once

#pragma once
#include <windows.h>
#include "..\\Register.h"
#include <time.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <queue>

#ifndef OPEARREGISTER_H
#define OPEARREGISTER_H

#ifdef REGISTER_DLL
#define OPEARREGISTER_API _declspec(dllexport)
#else
#define OPEARREGISTER_API _declspec(dllimport)
#endif

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

//#define COMMAND_OUTPUT

class CRegisterOpear :public CRegister
{
public:

	CRegisterOpear();
	~CRegisterOpear();

public:

	BOOL BanRegister();					// 禁止注册表
	BOOL ActiveRegister();				// 激活注册表
	void query(HKEY rootKey, const wchar_t* path);		// 查找
	void regQuery(HKEY beginKey, TCHAR* path);			// 查找--》调用Query

protected:

private:

	BOOL m_isBan;						// 注册表是否禁止 默认是FALSE
	std::queue<std::wstring> keystack;
	//overperpo
	DWORD dwType;
};

#endif

// 测试代码
// 遍历
//regQuery(HKEY_LOCAL_MACHINE, L"SOFTWARE");
//fs.close();
