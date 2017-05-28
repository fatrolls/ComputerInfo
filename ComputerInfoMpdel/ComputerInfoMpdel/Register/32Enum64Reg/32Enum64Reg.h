#pragma  once

/***************************************************************************************************/
/*    32位程序枚举64位系统注册表子项，含关闭重定向和提权                                                    */
/*        QueryKey - 枚举注册表子项和其关联的值.                                                         */
/* 		  hKey - 被枚举的注册表子项和值.                                                                */
/*        如何以 TrustedInstaller 用户权限修改注册表													    */
/* 		  通过代码将当前权限提升到 TrustedInstaller 很麻烦, 但是可以通过获取备份还原权限来绕过DACL的监测机制.     */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/***************************************************************************************************/

#pragma comment(lib,"advapi32")
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#define KEY_ALL_WOW64 (KEY_QUERY_VALUE | KEY_WOW64_64KEY)

bool EnablePriviledge(LPCTSTR lpSystemName) {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp = { 1 };
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		if (LookupPrivilegeValue(NULL, lpSystemName, &tkp.Privileges[0].Luid)) {
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			if (GetLastError() != ERROR_SUCCESS) {
				CloseHandle(hToken);
				return false;
			}
		}
		CloseHandle(hToken);
	}
	return true;
}

void QueryKey(HKEY hKey)
{
	TCHAR    achKey[MAX_KEY_LENGTH];            // 注册表子项名称缓冲区
	DWORD    cbName;                            // 名称字符串大小
	TCHAR    achClass[MAX_PATH] = TEXT("");     // 类名缓冲区
	DWORD    cchClassName = MAX_PATH;           // 类字符串大小
	DWORD    cSubKeys = 0;                      // 注册表子项数量
	DWORD    cbMaxSubKey;                       // 最长注册表子项大小
	DWORD    cchMaxClass;                       // 最长类字符串
	DWORD    cValues;                           // 项值数量（项内有多少个值）
	DWORD    cchMaxValue;                       // 最长值名称
	DWORD    cbMaxValueData;                    // 最长值数据
	DWORD    cbSecurityDescriptor;              // 安全描述符大小
	FILETIME ftLastWriteTime;                   // 最后写入时间

	DWORD i, retCode;

	TCHAR   achValue[MAX_VALUE_NAME];
	DWORD   cchValue = MAX_VALUE_NAME;

	// 获取类名和值数.
	retCode = RegQueryInfoKey(
		hKey,                    // 一个已打开项的句柄（由 RegOpenKeyEx 或 RegCreateKeyEx 得到）
		achClass,                // 类名缓冲区
		&cchClassName,           // 类字符串大小
		NULL,                    // 保留值, 设为 NULL
		&cSubKeys,               // 注册表子项数量
		&cbMaxSubKey,            // 最长注册表子项大小
		&cchMaxClass,            // 最长类字符串
		&cValues,                // 项值数量
		&cchMaxValue,            // 最长值名称
		&cbMaxValueData,         // 最长值数据
		&cbSecurityDescriptor,   // 安全描述符
		&ftLastWriteTime);       // 最后写入时间

	// 枚举注册表子项, 直到 RegEnumKeyEx 方法失败.
	if (cSubKeys)
	{
		printf("\n子项数量: %d\n", cSubKeys);
		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx( // 调用 RegEnumKeyExA 时 如果老返回 5 访问拒绝, 可尝试 RegCreateKeyEx
				hKey,
				i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,   // 调用 RegEnumKeyExA 时, 此值设为 MAX_PATH 可解决方法返回 234 错误问题.（貌似非 NULL 都可以）
				// 例如 DWORD cNameLen [MAX_PATH]; RegEnumKeyExA(hKey, i, achKey, cbName, NULL, NULL, cNameLen, &ftLastWriteTime);
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
			}
		}
	}

	// 枚举项值.

	if (cValues)
	{
		printf("\n值数量: %d\n", cValues);

		for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
		{
			cchValue = MAX_VALUE_NAME;
			achValue[0] = '\0';
			retCode = RegEnumValue(
				hKey,
				i,
				achValue,
				&cchValue,
				NULL,
				NULL,
				NULL,
				NULL);
			if (retCode == ERROR_SUCCESS)
			{
				_tprintf(TEXT("(%d) %s\n"), i + 1, achValue);
			}
		}
	}
}

int EnumReg32To64() {
	bool bRet;
	LONG lResult;
	PVOID oldWow64State = NULL; // 无类型指针
	bRet = EnablePriviledge(SE_BACKUP_NAME);    // 这个函数让当前进程具有备份/还原的特权.
	if (bRet) {
		bRet = EnablePriviledge(SE_RESTORE_NAME);
		if (bRet) {
			HKEY hResult = NULL;
			DWORD dwDisposition;
			if (Wow64DisableWow64FsRedirection(&oldWow64State)){
				/*lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,    // 使用 RegOpenKeyEx 会因为权限不够无法访问部分需要高权限的注册项
				TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkList\\Profiles"),
				0,
				KEY_ALL_WOW64,
				&hResult
				);
				*/

				lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
					TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkList\\Profiles"),
					0,
					NULL,
					REG_OPTION_BACKUP_RESTORE,      // 这个是重点, 传入这个参数可以直接忽视 KEY_ALL_ACCESS 这个参数的作用, 直接以备份/还原的特权去操作注册表
					KEY_ALL_WOW64,                  // 64位系统用 KEY_ALL_ACCESS 会无法访问 64 位注册项, 关闭重定向也会失败
					NULL,
					&hResult,
					&dwDisposition);

				RegDisableReflectionKey(hResult);
				QueryKey(hResult);
				RegCloseKey(hResult);
				RegEnableReflectionKey(hResult);
			}
			Wow64RevertWow64FsRedirection(oldWow64State);
			if (lResult != ERROR_SUCCESS) {
				return 3;
			}
			getchar();
			return 0;
		}
		else return 2;
	}
	else return 1;
}