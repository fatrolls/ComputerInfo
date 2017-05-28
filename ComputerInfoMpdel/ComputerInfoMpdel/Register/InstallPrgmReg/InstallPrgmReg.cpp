#pragma  once
#include"stdafx.h"
#include"InstallPrgmReg.h"

CInstallPgmReg::~CInstallPgmReg()
{

}	


CInstallPgmReg::CInstallPgmReg()
{
	struct SoftInfo softinfo;
	HKEY RootKey;            // 主键  
	LPCTSTR lpSubKey;        // 子键名称  
	HKEY hkResult;            // 将要打开键的句柄   
	HKEY hkRKey;
	LONG lReturn;            // 记录读取注册表是否成功  
	CString strBuffer;
	CString strMidReg;

	DWORD index = 0;
	TCHAR szKeyName[255] = { 0 };        // 注册表项名称  
	TCHAR szBuffer[255] = { 0 };
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = REG_BINARY | REG_DWORD | REG_EXPAND_SZ | REG_MULTI_SZ | REG_NONE | REG_SZ;
	RootKey = HKEY_LOCAL_MACHINE;
	lpSubKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

	lReturn = RegOpenKeyEx(RootKey, lpSubKey, 0, KEY_ALL_ACCESS, &hkResult);

	if (lReturn == ERROR_SUCCESS)
	{

		while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hkResult, index, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
		{
			index++;
			strBuffer.Format(_T("%s"), szKeyName);
			if (!strBuffer.IsEmpty())
			{
				strMidReg = (CString)lpSubKey + _T("\\") + strBuffer;

				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strMidReg, 0, KEY_ALL_ACCESS, &hkRKey) == ERROR_SUCCESS)
				{
					RegQueryValueEx(hkRKey, _T("DisplayName"), 0, &dwType, (LPBYTE)szBuffer, &dwNameLen);
					softinfo.m_strSoftName = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					RegQueryValueEx(hkRKey, _T("DisplayVersion"), 0, &dwType, (LPBYTE)szBuffer, &dwNameLen);
					softinfo.m_strSoftVersion = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					RegQueryValueEx(hkRKey, _T("InstallLocation"), 0, &dwType, (LPBYTE)szBuffer, &dwNameLen);
					softinfo.m_strInstallLocation = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					RegQueryValueEx(hkRKey, _T("Publisher"), 0, &dwType, (LPBYTE)szBuffer, &dwNameLen);
					softinfo.m_strPublisher = szBuffer;
					dwNameLen = 255;

					RegQueryValueEx(hkRKey, _T("InstallLocation"), 0, &dwType, (LPBYTE)szBuffer, &dwNameLen);
					softinfo.m_strMainProPath = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					RegQueryValueEx(hkRKey, _T("UninstallString"), 0, &dwType, (LPBYTE)szBuffer, &dwNameLen);
					softinfo.m_strUninstallPth = szBuffer;
					dwNameLen = 255;
					memset(szBuffer, 0, 255);

					if (!softinfo.m_strSoftName.IsEmpty())
					{
						if (strBuffer.GetAt(0) == 'K' && strBuffer.GetAt(1) == 'B')
						{
							m_SystemPatchesArr.push_back(softinfo);
						}
						else
						{
							m_SoftInfoArr.push_back(softinfo);
						}
					}
				}

				dwKeyLen = 255;
				memset(szKeyName, 0, 255);
			}
		}
		RegCloseKey(hkResult);
	}
	else
	{
		::MessageBox(NULL, _T("打开注册表失败!"), NULL, MB_ICONWARNING);
	}
}

std::vector<SoftInfo> CInstallPgmReg::GetSoftInfo(void) const
{
	return m_SoftInfoArr;
}

void CInstallPgmReg::GetSoftName(std::vector<LPCTSTR>& lpszSoftName)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		lpszSoftName.push_back(iter->m_strSoftName);
	}
}

void CInstallPgmReg::GetSoftVersion(std::vector<LPCTSTR>& lpszSoftVersion)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		if (!(iter->m_strSoftVersion).IsEmpty())
		{
			lpszSoftVersion.push_back(iter->m_strSoftVersion);
		}
	}
}

void CInstallPgmReg::GetInstallLocation(std::vector<LPCTSTR>& lpszInstallLocation)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		if (!(iter->m_strInstallLocation).IsEmpty())
		{
			lpszInstallLocation.push_back(iter->m_strInstallLocation);
		}
	}
}

void CInstallPgmReg::GetPublisher(std::vector<LPCTSTR>& lpszPublisher)
{
	std::vector<SoftInfo>::iterator iter;

	bool bSign;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		bSign = true;

		// 初步去重复厂商  
		std::vector<LPCTSTR>::iterator itr;
		for (itr = lpszPublisher.begin(); itr != lpszPublisher.end(); itr++)
		{
			if (iter->m_strPublisher == (CString)*itr)
			{
				bSign = false;
			}
		}

		if (bSign)
		{
			lpszPublisher.push_back(iter->m_strPublisher);
		}
	}
}

void CInstallPgmReg::GetMainProPath(std::vector<LPCTSTR>& lpszMainProPath)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		if (!(iter->m_strMainProPath).IsEmpty())
		{
			lpszMainProPath.push_back(iter->m_strMainProPath);
		}
	}
}

void CInstallPgmReg::GetUninstallPth(std::vector<LPCTSTR>& lpszSoftName)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SoftInfoArr.begin(); iter != m_SoftInfoArr.end(); iter++)
	{
		if (!(iter->m_strUninstallPth).IsEmpty())
		{
			lpszSoftName.push_back(iter->m_strUninstallPth);
		}
	}
}

std::vector<SoftInfo> CInstallPgmReg::GetSystemPatchesInfo(void) const
{
	return m_SystemPatchesArr;
}

void CInstallPgmReg::GetSystemPatchesName(std::vector<LPCTSTR>& lpszSoftName)
{
	std::vector<SoftInfo>::iterator iter;
	for (iter = m_SystemPatchesArr.begin(); iter != m_SystemPatchesArr.end(); iter++)
	{
		lpszSoftName.push_back(iter->m_strSoftName);
	}
}