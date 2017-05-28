#ifndef REGISTER_H
#define REGISTER_H

#pragma once
#include <windows.h>

#ifdef REGISTER_DLL
#define REGISTERAPI _declspec(dllexport)
#else
#define REGISTERAPI _declspec(dllimport)
#endif

// ���ݶ���
#define ERROR_FAILED							0x000000000							// ʧ�ܴ���

#define HREGROOTKEY  HKEY															// �ض���root���
#define HREGKEY	HKEY																// ���¶�����
#define HKEY_REG_CLASSES_ROOT					HKEY_CLASSES_ROOT					// root
#define HKEY_REG_CURRENT_USER					HKEY_CURRENT_USER					// current_user
#define HKEY_REG_LOCAL_MACHINE					HKEY_LOCAL_MACHINE					// machine
#define HKEY_REG_USERS							HKEY_USERS							// user
#define HKEY_REG_PERFORMANCE_DATA				HKEY_PERFORMANCE_DATA				// performance_data
#define HKEY_REG_PERFORMANCE_TEXT				HKEY_PERFORMANCE_TEXT				// performance_text
#define HKEY_REG_PERFORMANCE_NLSTEXT			HKEY_PERFORMANCE_NLSTEXT			// performance_nlstext

#if (WINVER >= 0x0400)
#define HKEY_REG_CURRENT_CONFIG					HKEY_CURRENT_CONFIG					// current_config
#define HKEY_REG_DYN_DATA						HKEY_DYN_DATA						// dyn_data
#define HKEY_REG_CURRENT_USER_LOCAL_SETTINGS	HKEY_CURRENT_USER_LOCAL_SETTINGS    // current_user_local_setttings
#endif

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct REGCREATEKEYPARAM
	{
		_In_       HKEY                  _hKey;										// A handle to an open registry key.
		_In_       LPCTSTR               _lpSubKey;									// The name of a subkey that this function opens or creates.
		_Reserved_ DWORD                 _Reserved = 0;								// This parameter is reserved and must be zero.
		_In_opt_   LPTSTR                _lpClass;									// The user-defined class type of this key.
		_In_       DWORD                 _dwOptions;								// This parameter can be one of the following values  REG_OPTION_BACKUP_RESTORE|REG_OPTION_CREATE_LINK|REG_OPTION_NON_VOLATILE|REG_OPTION_VOLATILE
		_In_       REGSAM                _samDesired;								// A mask that specifies the access rights for the key to be created.
		_In_opt_   LPSECURITY_ATTRIBUTES _lpSecurityAttributes;						// A pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned handle can be inherited by child processes
		_Out_      PHKEY                 _phkResult;								// A pointer to a variable that receives a handle to the opened or created key.
		_Out_opt_  LPDWORD               _lpdwDisposition;							// A pointer to a variable that receives one of the following disposition values. REG_CREATED_NEW_KEY |REG_OPENED_EXISTING_KEY
	}_RegCreateKeyParam, *_lpRegCeateKeyParam;

	class CRegister
	{
	public:

		CRegister();
		CRegister(HREGKEY hkey);
		~CRegister();

	public:

		static  HREGKEY CreateRegKey(HREGROOTKEY _hkey, LPCTSTR subkey,
			REGCREATEKEYPARAM* _lpRegCeateKeyParam); 			// ������ֵ

		static  HREGKEY OpenRegKey(HREGROOTKEY _hkey, LPCTSTR _subkey,
			DWORD ulOptions = 0L, REGSAM samDesired = KEY_ALL_ACCESS);   					// ��д��ʽ��ע��� ���ش��ӽ��ľ�� hkey
		bool IsRegValueExisted(HREGROOTKEY _hkey, LPCTSTR _subkey, LPCTSTR pValName);   	// ��д��ʽ��ע��� ����bool ��ֵ�Ƿ����
		long IsRegItemExited(HREGROOTKEY _hkey, LPCSTR _subkey);							// ע������ǲ��Ǵ���

		void CloseReg() { RegCloseKey(m_hrootkey); m_hrootkey = nullptr; };					// ������Դ
		void CloseReg(HREGKEY hkey){ RegCloseKey(hkey); };

		static bool SaveRegKey(HREGROOTKEY _hkey, LPCTSTR lpFileName,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr);  						// �����ֵ���ļ���

		static bool RestoreRegKey(HREGROOTKEY _hkey, LPCTSTR lpFileName,
			DWORD dwFlags = REG_WHOLE_HIVE_VOLATILE);  										// ��ָ��ע����ļ��лָ�

		static	bool DeleteRegKey(HREGKEY _hkey, LPCTSTR lpSubKey); 						// ɾ����Ӧ�Ӽ�

		static	bool DeleteRegValue(HREGKEY _hkey, LPCTSTR lpValueName);   					// ɾ���Ӽ������Ӧ�ļ�ֵ

		static  bool ReadReg(HREGKEY _hkey, LPCTSTR lpValueName,
			LPTSTR* lpszVal); 																// ��ȡREG_SZ���ͼ�ֵ

		static	bool ReadReg(HREGKEY _hkey, LPCTSTR lpValueName,
			DWORD* lpdwVal);   																// ��ȡDWORD���ͼ�ֵ

		static	bool ReadReg(HREGKEY _hkey, LPCTSTR lpValueName,
			int* lpnVal);  																		// ��ȡint���͵ļ�ֵ

		static	bool WriteReg(HREGKEY _hkey, LPCTSTR  lpValueName,
			LPCTSTR  lpValue);   															// д��REG_SZ���ͼ�ֵ

		static	bool WriteReg(HREGKEY _hkey, LPCTSTR lpSubKey,
			DWORD lpdwValue);  																// д��DWORD���ͼ�ֵ

		static	bool WriteReg(HREGKEY _hkey, LPCTSTR lpSubKey, int nval);  					// д��int���ͼ�ֵ

		HREGROOTKEY GetBaseRegKey(char* keystr);											// ת��������

	protected:

		HREGROOTKEY GetRootKey() const { return m_hrootkey; }								// ��ȡ�������
		void SetRootKey(HREGKEY hkey){ m_hrootkey = hkey; }									// ���ø������

	private:

		HREGROOTKEY m_hrootkey;
	};

#ifdef __cplusplus
}
#endif

#endif