#ifndef REGISTER_H
#define REGISTER_H

#pragma once
#include <windows.h>

#ifdef REGISTER_DLL
#define REGISTERAPI _declspec(dllexport)
#else
#define REGISTERAPI _declspec(dllimport)
#endif

// 数据定义
#define ERROR_FAILED							0x000000000							// 失败代码

#define HREGROOTKEY  HKEY															// 重定义root句柄
#define HREGKEY	HKEY																// 重新定义句柄
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
			REGCREATEKEYPARAM* _lpRegCeateKeyParam); 			// 创建键值

		static  HREGKEY OpenRegKey(HREGROOTKEY _hkey, LPCTSTR _subkey,
			DWORD ulOptions = 0L, REGSAM samDesired = KEY_ALL_ACCESS);   					// 读写形式打开注册表 返回打开子健的句柄 hkey
		bool IsRegValueExisted(HREGROOTKEY _hkey, LPCTSTR _subkey, LPCTSTR pValName);   	// 读写形式打开注册表 返回bool 键值是否存在
		long IsRegItemExited(HREGROOTKEY _hkey, LPCSTR _subkey);							// 注册表项是不是存在

		void CloseReg() { RegCloseKey(m_hrootkey); m_hrootkey = nullptr; };					// 清理资源
		void CloseReg(HREGKEY hkey){ RegCloseKey(hkey); };

		static bool SaveRegKey(HREGROOTKEY _hkey, LPCTSTR lpFileName,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr);  						// 保存键值到文件中

		static bool RestoreRegKey(HREGROOTKEY _hkey, LPCTSTR lpFileName,
			DWORD dwFlags = REG_WHOLE_HIVE_VOLATILE);  										// 从指定注册表文件中恢复

		static	bool DeleteRegKey(HREGKEY _hkey, LPCTSTR lpSubKey); 						// 删除对应子键

		static	bool DeleteRegValue(HREGKEY _hkey, LPCTSTR lpValueName);   					// 删除子键处相对应的键值

		static  bool ReadReg(HREGKEY _hkey, LPCTSTR lpValueName,
			LPTSTR* lpszVal); 																// 读取REG_SZ类型键值

		static	bool ReadReg(HREGKEY _hkey, LPCTSTR lpValueName,
			DWORD* lpdwVal);   																// 读取DWORD类型键值

		static	bool ReadReg(HREGKEY _hkey, LPCTSTR lpValueName,
			int* lpnVal);  																		// 读取int类型的键值

		static	bool WriteReg(HREGKEY _hkey, LPCTSTR  lpValueName,
			LPCTSTR  lpValue);   															// 写入REG_SZ类型键值

		static	bool WriteReg(HREGKEY _hkey, LPCTSTR lpSubKey,
			DWORD lpdwValue);  																// 写入DWORD类型键值

		static	bool WriteReg(HREGKEY _hkey, LPCTSTR lpSubKey, int nval);  					// 写入int类型键值

		HREGROOTKEY GetBaseRegKey(char* keystr);											// 转换基础键

	protected:

		HREGROOTKEY GetRootKey() const { return m_hrootkey; }								// 获取根键句柄
		void SetRootKey(HREGKEY hkey){ m_hrootkey = hkey; }									// 设置根键句柄

	private:

		HREGROOTKEY m_hrootkey;
	};

#ifdef __cplusplus
}
#endif

#endif