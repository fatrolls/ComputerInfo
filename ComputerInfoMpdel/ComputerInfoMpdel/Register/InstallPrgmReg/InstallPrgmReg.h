/***************************************************************************************************/
/*                               ��ע����ѯ���Ѱ�װ����                                               */
/*   1�� �����������汾�ţ������װĿ¼������������̣���������������·����ж��exe��������·��                   */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/***************************************************************************************************/

#pragma  once

#ifndef INTALLPRGMREG_H
#define INTALLPRGMREG_H


#pragma once
#include <windows.h>
#include "..\OpearRegister\OpearRegister.h"
#include <vector>
#include <afxstr.h>


#ifdef REGISTER_DLL
#define INTALLPRGMREG_API _declspec(dllexport)
#else
#define INTALLPRGMREG_API _declspec(dllimport)
#endif

struct SoftInfo
{
	// �����  
	CString m_strSoftName;
	// ����汾��  
	CString m_strSoftVersion;
	// �����װĿ¼  
	CString m_strInstallLocation;
	// �����������  
	CString m_strPublisher;
	// ��������������·��  
	CString m_strMainProPath;
	// ж��exe��������·��  
	CString m_strUninstallPth;
};


class CInstallPgmReg :public CRegisterOpear
{
public:
	CInstallPgmReg();
	~CInstallPgmReg();
public:
	// ��ȡһ���������������װ��Ϣ��Vector  
	std::vector<SoftInfo> GetSoftInfo(void) const;
	// ��ȡ�����Ѱ�װ���������  
	void GetSoftName(std::vector<LPCTSTR>& lpszSoftName);
	// ��ȡ�����Ѱ�װ��������汾��  
	void GetSoftVersion(std::vector<LPCTSTR>& lpszSoftVersion);
	// ��ȡ�����Ѱ�װ���������װĿ¼  
	void GetInstallLocation(std::vector<LPCTSTR>& lpszInstallLocation);
	// ��ȡ�����Ѱ�װ���������������  
	void GetPublisher(std::vector<LPCTSTR>& lpszPublisher);
	// ��ȡ�����Ѱ�װ�����������������·��  
	void GetMainProPath(std::vector<LPCTSTR>& lpszMainProPath);
	// ��ȡ�����Ѱ�װ�������ж�س�������·��  
	void GetUninstallPth(std::vector<LPCTSTR>& lpszSoftName);

	// ��ȡһ������ϵͳ������Ϣ��Vector  
	std::vector<SoftInfo> GetSystemPatchesInfo(void) const;
	// ��ȡ�����Ѱ�װϵͳ������  
	void GetSystemPatchesName(std::vector<LPCTSTR>& lpszSoftName);

protected:

private:
	// �����Ѱ�װ���������װ��Ϣ  
	std::vector<SoftInfo> m_SoftInfoArr;
	// ����ϵͳ������Ϣ  
	std::vector<SoftInfo> m_SystemPatchesArr;

};





#endif
