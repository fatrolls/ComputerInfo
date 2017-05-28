#pragma once

#include "stdafx.h"
#include "Register.h"
#include <cassert>

#ifndef	_REGISTER_CPP
#define _REGISTER_CPP

CRegister::CRegister()
{
	SetRootKey(HKEY_LOCAL_MACHINE);
}

CRegister::CRegister(HREGKEY hkey)
{
	SetRootKey(hkey);
}

CRegister::~CRegister()
{
	CloseReg();
}

// ������ֵ
HREGKEY CRegister::CreateRegKey(HREGROOTKEY _hkey, LPCTSTR subkey, REGCREATEKEYPARAM* _lpRegCeateKeyParam)
{
	assert(_hkey);
	assert(subkey);

	LONG lReturn = RegCreateKeyEx(_hkey, subkey,
		_lpRegCeateKeyParam->_Reserved,
		_lpRegCeateKeyParam->_lpClass,
		_lpRegCeateKeyParam->_dwOptions,
		_lpRegCeateKeyParam->_samDesired,
		_lpRegCeateKeyParam->_lpSecurityAttributes,
		&_lpRegCeateKeyParam->_hKey,
		_lpRegCeateKeyParam->_lpdwDisposition);

	if (lReturn == ERROR_SUCCESS)
	{
		return _lpRegCeateKeyParam->_hKey;
	}
	return _lpRegCeateKeyParam->_hKey = ERROR_FAILED;
}

// ��д��ʽ��ע���
HREGKEY CRegister::OpenRegKey(HREGROOTKEY _hkey, LPCTSTR _subkey, DWORD ulOptions, REGSAM samDesired)
{
	assert(_hkey);
	assert(_subkey);

	HREGKEY hKey;
	long lReturn = RegOpenKeyEx(_hkey, _subkey, ulOptions, samDesired, &hKey);

	if (lReturn == ERROR_SUCCESS)
	{
		return hKey;
	}
	return hKey = ERROR_FAILED;
}

// IsRegValueExisted(HKEY_CURRENT_USER ,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("CCUI")

// ��ֵ�Ƿ����
bool CRegister::IsRegValueExisted(HREGROOTKEY _hkey, LPCTSTR _subkey, LPCTSTR pValName)
{
	assert(_hkey);
	assert(_subkey);

	bool bRet = false;
	DWORD dwType = REG_SZ;
	HKEY hKey;
	LSTATUS nRes = RegOpenKeyEx(_hkey, _subkey, 0, KEY_READ, &hKey);
	if (nRes != ERROR_SUCCESS) {
		return false;
	}
	nRes = RegQueryValueEx(hKey, pValName, NULL, &dwType, NULL, NULL);
	RegCloseKey(hKey);
	if (nRes == ERROR_SUCCESS || nRes == ERROR_MORE_DATA) {
		bRet = true;
	}
	return bRet;
}
// ע������ǲ��Ǵ���
long IsRegItemExited(HREGROOTKEY _hkey, LPCSTR _subkey)
{
	assert(_hkey);
	assert(_subkey);
	HREGKEY hKey;
	return RegOpenKeyEx(_hkey, (LPCWSTR)_subkey, 0UL, KEY_ALL_ACCESS, &hKey);
}

// �����ֵ���ļ���
// lpSecurityAttributes [in, optional]
// A pointer to a SECURITY_ATTRIBUTES structure that specifies a security descriptor for the new file.If lpSecurityAttributes is nullptr,
// the file gets a default security descriptor.The ACLs in a default security descriptor for a file are inherited from its parent directory.

bool CRegister::SaveRegKey(HREGROOTKEY _hkey, LPCTSTR lpFileName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	assert(_hkey);
	assert(lpFileName);

	long lReturn = RegSaveKey(_hkey, lpFileName, lpSecurityAttributes);

	if (lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

// ��ָ��ע����ļ��лָ�

bool CRegister::RestoreRegKey(HREGROOTKEY _hkey, LPCTSTR lpFileName, DWORD dwFlags)
{
	assert(_hkey);
	assert(lpFileName);

	long lReturn = RegRestoreKey(_hkey, lpFileName, dwFlags);

	if (lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

// ɾ����Ӧ�Ӽ�
// _hkey  ����root �ľ�����Ƕ�ӦҪɾ���ľ��

bool CRegister::DeleteRegKey(HREGKEY _hkey, LPCTSTR lpSubKey)
{
	assert(_hkey);
	assert(lpSubKey);

	long lReturn = RegDeleteValue(_hkey, lpSubKey);

	if (lReturn == ERROR_SUCCESS)
		return TRUE;
	return FALSE;
}

// ɾ���Ӽ������Ӧ�ļ�ֵ

bool CRegister::DeleteRegValue(HREGKEY _hkey, LPCTSTR lpValueName)
{
	assert(_hkey);
	assert(lpValueName);

	long lReturn = RegDeleteValue(_hkey, lpValueName);

	if (lReturn == ERROR_SUCCESS)
		return TRUE;
	return FALSE;
}

// ��ȡREG_SZ���ͼ�ֵ

bool CRegister::ReadReg(HREGKEY _hkey, LPCTSTR lpValueName, LPTSTR* lpszVal)
{
	assert(_hkey);
	assert(lpValueName);
	/*assert(lpszVal);
	lpszVal = 0;*/

	DWORD dwType;
	DWORD dwSize = 200;
	char szString[2550];

	long lReturn = RegQueryValueEx(_hkey, lpValueName, nullptr, &dwType, (BYTE *)szString, &dwSize);

	if (lReturn == ERROR_SUCCESS)
	{
		*lpszVal = (LPTSTR)szString;
		return true;
	}
	return false;
}

// ��ȡDWORD���ͼ�ֵ REG_DWORD

bool CRegister::ReadReg(HREGKEY _hkey, LPCTSTR lpValueName, DWORD* lpdwVal)
{
	assert(_hkey);
	assert(lpValueName);
	assert(lpdwVal);

	DWORD dwType;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwDest;
	long lReturn = RegQueryValueEx(_hkey, lpValueName, nullptr, &dwType, (BYTE *)&dwDest, &dwSize);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ��ѯ�ɹ� */
		*lpdwVal = dwSize;
		return true;
	}
	/* ��ѯʧ�� */
	return false;
}

// ��ȡint���͵ļ�ֵ

bool CRegister::ReadReg(HREGKEY _hkey, LPCTSTR lpValueName, int* lpnVal)
{
	assert(_hkey);
	assert(lpValueName);

	DWORD dwType;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwDest;
	long lReturn = RegQueryValueEx(_hkey, lpValueName, nullptr, &dwType, (BYTE *)&dwDest, &dwSize);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ��ѯ�ɹ� */
		lpnVal = (int *)&dwDest;    // ���ֱ���   ���־ֲ��������ݶ�ʧ
		return true;
	}
	return false;
}

// д��REG_SZ���ͼ�ֵ

bool CRegister::WriteReg(HREGKEY _hkey, LPCTSTR  lpValueName, LPCTSTR  lpValue)
{
	assert(_hkey);
	assert(lpValueName);
	assert(lpValue);

	long lReturn = RegSetValueEx(_hkey, lpValueName, 0L, REG_SZ, (const BYTE *)lpValue, strlen((const char *)lpValue) + 1);

	if (ERROR_SUCCESS == lReturn)
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;
}

// д��DWORD���ͼ�ֵ

bool CRegister::WriteReg(HREGKEY _hkey, LPCTSTR lpSubKey, DWORD dwValue)
{
	assert(_hkey);
	assert(lpSubKey);

	long lReturn = RegSetValueEx(_hkey, lpSubKey, 0L, REG_DWORD, (const BYTE *)&dwValue, sizeof(DWORD));

	if (ERROR_SUCCESS == lReturn)
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;
}

// д��int���ͼ�ֵ

bool CRegister::WriteReg(HREGKEY _hkey, LPCTSTR lpSubKey, int nVal)
{
	assert(_hkey);
	assert(lpSubKey);

	DWORD dwValue;
	dwValue = (DWORD)nVal;

	long lReturn = RegSetValueEx(_hkey, lpSubKey, 0L, REG_DWORD, (const BYTE *)&dwValue, sizeof(DWORD));

	if (ERROR_SUCCESS == lReturn)
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;
}

//-- ת��������
HREGROOTKEY CRegister::GetBaseRegKey(char* keystr)
{
	HKEY hKey = nullptr;
	if (strcmp(keystr, "HKEY_CLASSES_ROOT") == 0)
		hKey = HKEY_CLASSES_ROOT;
	if (strcmp(keystr, "HKEY_CURRENT_CONFIG") == 0)
		hKey = HKEY_CURRENT_CONFIG;
	if (strcmp(keystr, "HKEY_CURRENT_USER") == 0)    hKey = HKEY_CURRENT_USER;
	if (strcmp(keystr, "HKEY_LOCAL_MACHINE") == 0)   hKey = HKEY_LOCAL_MACHINE;
	if (strcmp(keystr, "HKEY_USERS") == 0)			 hKey = HKEY_USERS;
	return hKey;
}

#endif