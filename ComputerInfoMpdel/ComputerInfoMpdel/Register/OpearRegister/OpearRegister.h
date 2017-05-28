/************************************************************************/
/*    ע��������                                                        */
/*        1���̳���Register                                              */
/*        2�����ܵ㣺��ѯ�����ݣ�ɾ����ɾ���ָ��������������ӣ�               */
/*           ע����ֹ������                                              */
/*        3�����ݣ�1������������ݣ�2��ɾ��ǰ����                              */
/*        4����ѯ��													    */
/*				1��Ĭ�����е�ע������ѯ                                   */
/*				2��ָ����ѯע���Ŀ¼									    */
/*				3����ѯ������												*/
/*		  5�����������													*/
/*		  [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run]				*/
/*		  [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunOnce]��������һ�Σ� */
/*		  [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunServices]		*/
/*                                                                      */
/************************************************************************/
// ������
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

	BOOL BanRegister();					// ��ֹע���
	BOOL ActiveRegister();				// ����ע���
	void query(HKEY rootKey, const wchar_t* path);		// ����
	void regQuery(HKEY beginKey, TCHAR* path);			// ����--������Query

protected:

private:

	BOOL m_isBan;						// ע����Ƿ��ֹ Ĭ����FALSE
	std::queue<std::wstring> keystack;
	//overperpo
	DWORD dwType;
};

#endif

// ���Դ���
// ����
//regQuery(HKEY_LOCAL_MACHINE, L"SOFTWARE");
//fs.close();
