#pragma  once

/***************************************************************************************************/
/*    32λ����ö��64λϵͳע���������ر��ض������Ȩ                                                    */
/*        QueryKey - ö��ע���������������ֵ.                                                         */
/* 		  hKey - ��ö�ٵ�ע��������ֵ.                                                                */
/*        ����� TrustedInstaller �û�Ȩ���޸�ע���													    */
/* 		  ͨ�����뽫��ǰȨ�������� TrustedInstaller ���鷳, ���ǿ���ͨ����ȡ���ݻ�ԭȨ�����ƹ�DACL�ļ�����.     */
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
	TCHAR    achKey[MAX_KEY_LENGTH];            // ע����������ƻ�����
	DWORD    cbName;                            // �����ַ�����С
	TCHAR    achClass[MAX_PATH] = TEXT("");     // ����������
	DWORD    cchClassName = MAX_PATH;           // ���ַ�����С
	DWORD    cSubKeys = 0;                      // ע�����������
	DWORD    cbMaxSubKey;                       // �ע��������С
	DWORD    cchMaxClass;                       // ����ַ���
	DWORD    cValues;                           // ��ֵ�����������ж��ٸ�ֵ��
	DWORD    cchMaxValue;                       // �ֵ����
	DWORD    cbMaxValueData;                    // �ֵ����
	DWORD    cbSecurityDescriptor;              // ��ȫ��������С
	FILETIME ftLastWriteTime;                   // ���д��ʱ��

	DWORD i, retCode;

	TCHAR   achValue[MAX_VALUE_NAME];
	DWORD   cchValue = MAX_VALUE_NAME;

	// ��ȡ������ֵ��.
	retCode = RegQueryInfoKey(
		hKey,                    // һ���Ѵ���ľ������ RegOpenKeyEx �� RegCreateKeyEx �õ���
		achClass,                // ����������
		&cchClassName,           // ���ַ�����С
		NULL,                    // ����ֵ, ��Ϊ NULL
		&cSubKeys,               // ע�����������
		&cbMaxSubKey,            // �ע��������С
		&cchMaxClass,            // ����ַ���
		&cValues,                // ��ֵ����
		&cchMaxValue,            // �ֵ����
		&cbMaxValueData,         // �ֵ����
		&cbSecurityDescriptor,   // ��ȫ������
		&ftLastWriteTime);       // ���д��ʱ��

	// ö��ע�������, ֱ�� RegEnumKeyEx ����ʧ��.
	if (cSubKeys)
	{
		printf("\n��������: %d\n", cSubKeys);
		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx( // ���� RegEnumKeyExA ʱ ����Ϸ��� 5 ���ʾܾ�, �ɳ��� RegCreateKeyEx
				hKey,
				i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,   // ���� RegEnumKeyExA ʱ, ��ֵ��Ϊ MAX_PATH �ɽ���������� 234 ��������.��ò�Ʒ� NULL �����ԣ�
				// ���� DWORD cNameLen [MAX_PATH]; RegEnumKeyExA(hKey, i, achKey, cbName, NULL, NULL, cNameLen, &ftLastWriteTime);
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
			}
		}
	}

	// ö����ֵ.

	if (cValues)
	{
		printf("\nֵ����: %d\n", cValues);

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
	PVOID oldWow64State = NULL; // ������ָ��
	bRet = EnablePriviledge(SE_BACKUP_NAME);    // ��������õ�ǰ���̾��б���/��ԭ����Ȩ.
	if (bRet) {
		bRet = EnablePriviledge(SE_RESTORE_NAME);
		if (bRet) {
			HKEY hResult = NULL;
			DWORD dwDisposition;
			if (Wow64DisableWow64FsRedirection(&oldWow64State)){
				/*lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,    // ʹ�� RegOpenKeyEx ����ΪȨ�޲����޷����ʲ�����Ҫ��Ȩ�޵�ע����
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
					REG_OPTION_BACKUP_RESTORE,      // ������ص�, ���������������ֱ�Ӻ��� KEY_ALL_ACCESS �������������, ֱ���Ա���/��ԭ����Ȩȥ����ע���
					KEY_ALL_WOW64,                  // 64λϵͳ�� KEY_ALL_ACCESS ���޷����� 64 λע����, �ر��ض���Ҳ��ʧ��
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