#include "stdafx.h"
#include <initguid.h>
#include <comutil.h>
#include <iostream>
using namespace std;

#include "WinManageInstrumentation.h"
#include "..\Define.h"
#include "..\Exception\Exception.h"


CWinManageInstrumentation::CWinManageInstrumentation() :m_pLoc(nullptr), m_pSvc(nullptr), m_pEnumerator(nullptr)
{

}

CWinManageInstrumentation::~CWinManageInstrumentation()
{
	m_pSvc->Release();
	m_pLoc->Release();
	CoUninitialize();
//	delete m_pLoc;
//	delete m_pSvc;
//	delete m_pEnumerator;
}


// hres 
//S_OK:COM库初始化成功。
//S_FALSE : 当前线程上，COM库已经被初始化。
//RPC_E_CHANGED_MODE : COM库已经被初始化且传入参数设置的并发模式和本次不同。

BOOL CWinManageInstrumentation::WMICoInitializeEx(DWORD dwCoInit /* = COINIT_MULTITHREADED*/)
{
	HRESULT hres;
	// Initialize COM.
	try
	{
		hres = CoInitializeEx(0, dwCoInit);
		if (FAILED(hres))
		{
			cout << "Failed to initialize COM library. "
				<< "Error code = 0x"
				<< hex << hres << endl;
			return FALSE;              // Program has failed.  如何判断
		}
	}
	catch (ExceptionClass* e)
	{
		e->ReportError();
		return FALSE;
	}
	catch (...)
	{
		std::cout << ExceptionClass::GetLastErrorToString(GetLastError()).c_str() << endl;
		return FALSE;
	}
	return TRUE;
}

BOOL CWinManageInstrumentation::WMICoInitializeSecurity(WMICOINITSECURITYPARAM *_lpWMICoInitSecurityParam)
{
	try{
		// Initialize 
		HRESULT hres = CoInitializeSecurity(
			_lpWMICoInitSecurityParam->_pSecDesc,
			_lpWMICoInitSecurityParam->_cAuthSvc,
			_lpWMICoInitSecurityParam->_asAuthSvc,
			_lpWMICoInitSecurityParam->_pReserved1,
			_lpWMICoInitSecurityParam->_dwAuthnLevel,
			_lpWMICoInitSecurityParam->_dwImpLevel,
			_lpWMICoInitSecurityParam->_pAuthList,
			_lpWMICoInitSecurityParam->_dwCapabilities,
			_lpWMICoInitSecurityParam->_pReserved3
			);


		if (FAILED(hres))
		{
			cout << "Failed to initialize security. "
				<< "Error code = 0x"
				<< hex << hres << endl;
			CoUninitialize();
			return FALSE;          // Program has failed.
		}
	}
	catch (ExceptionClass* e)
	{
		e->ReportError();
		CoUninitialize();
		return FALSE;
	}
	catch (...)
	{
		std::cout << ExceptionClass::GetLastErrorToString(GetLastError()).c_str() << endl;
		return FALSE;
	}
	return TRUE;
}

// Obtain the initial locator to Windows Management
// on a particular host computer.
// 参数写死
BOOL CWinManageInstrumentation::WMICoCreateInstance()
{
	try{
		HRESULT hres = CoCreateInstance(
			CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID *)&m_pLoc);

		if (FAILED(hres))
		{
			cout << "Failed to create IWbemLocator object. "
				<< "Error code = 0x"
				<< hex << hres << endl;
			CoUninitialize();
			return FALSE;       // Program has failed.
		}
	}
	catch (ExceptionClass* e)
	{
		e->ReportError();
		CoUninitialize();
		return FALSE;
	}
	catch (...)
	{
		std::cout << ExceptionClass::GetLastErrorToString(GetLastError()).c_str() << endl;
		return FALSE;
	}
	return TRUE;
}

// Connect to the root\cimv2 namespace with the
// current user and obtain pointer pSvc
// to make IWbemServices calls.
BOOL CWinManageInstrumentation::WMIConnectServer(WMICONNECTSERVERPARAM *_lpWMIConnectServerParam)
{
	try{
		assert(m_pLoc);

		HRESULT hres = m_pLoc->ConnectServer(
			_lpWMIConnectServerParam->_strNetworkResourceConn, // WMI namespace
			//_bstr_t("ROOT\\CIMV2"),
			_lpWMIConnectServerParam->_strUser,                    // User name
			_lpWMIConnectServerParam->_strPassword,                    // User password
			_lpWMIConnectServerParam->_strLocale,                       // Localeq
			_lpWMIConnectServerParam->_lSecurityFlags,                    // Security flags                 
			_lpWMIConnectServerParam->_strAuthority,                       // Authority       
			_lpWMIConnectServerParam->_pCtx,                       // Context object
			&m_pSvc                    // IWbemServices proxy
			);
		if (FAILED(hres))
		{
			cout << "Could not connect. Error code = 0x"
				<< hex << hres << endl;
			m_pLoc->Release();
			CoUninitialize();
			return FALSE;                // Program has failed.
		}
	}
	catch (ExceptionClass* e)
	{
		e->ReportError();
		m_pLoc->Release();
		CoUninitialize();
		return FALSE;
	}
	catch (...)
	{
		std::cout << ExceptionClass::GetLastErrorToString(GetLastError()).c_str() << endl;
		return FALSE;
	}
	cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;
	return TRUE;
}

BOOL CWinManageInstrumentation::WMICoSetProxyBlanket(WMICOSETPROXYBLANKETPARAM* _lpWMICoSetProxyBlanketParam)			// 代理服务器
{
	try{
		assert(m_pSvc);
		assert(m_pLoc);
		HRESULT	hres = CoSetProxyBlanket(
			m_pSvc,                         // the proxy to set
			_lpWMICoSetProxyBlanketParam->_dwAuthnSvc,                        // authentication service
			_lpWMICoSetProxyBlanketParam->_dwAuthzSvc,                        // authorization service
			_lpWMICoSetProxyBlanketParam->_pServerPrincName,                  // Server principal name
			_lpWMICoSetProxyBlanketParam->_dwAuthnLevel,					  // authentication level
			_lpWMICoSetProxyBlanketParam->_dwImpLevel,						  // impersonation level
			_lpWMICoSetProxyBlanketParam->_pAuthInfo,                         // client identity 
			_lpWMICoSetProxyBlanketParam->_dwCapabilities                     // proxy capabilities     
			);
		if (FAILED(hres))
		{
			cout << "Could not set proxy blanket. Error code = 0x"
				<< hex << hres << endl;
			m_pSvc->Release();
			m_pLoc->Release();
			CoUninitialize();
			return FALSE;               // Program has failed.
		}
	}
	catch (ExceptionClass* e)
	{
		e->ReportError();
		m_pLoc->Release();
		CoUninitialize();
		return FALSE;
	}
	catch (...)
	{
		std::cout << ExceptionClass::GetLastErrorToString(GetLastError()).c_str() << endl;
		return FALSE;
	}
	return TRUE;
}

BOOL CWinManageInstrumentation::WMIExecQuery(BSTR  _strQueryLanguage, BSTR _strQuery, WMIQUERYPARAM* _lpWMIQueryParam)											// 查询 定义查询类型
{
	try{
		assert(m_pSvc);
		assert(m_pLoc);
		HRESULT	hres = m_pSvc->ExecQuery(
			_strQueryLanguage,
			_strQuery,
			_lpWMIQueryParam->_lFlags,
			_lpWMIQueryParam->_pCtx,
			&m_pEnumerator);

		if (FAILED(hres))
		{
			cout << "Query for processes failed. "
				<< "Error code = 0x"
				<< hex << hres << endl;
			m_pSvc->Release();
			m_pLoc->Release();
			CoUninitialize();
			return FALSE;               // Program has failed.
		}
	}
	catch (ExceptionClass* e)
	{
		e->ReportError();
		m_pLoc->Release();
		CoUninitialize();
		return FALSE;
	}
	catch (...)
	{
		std::cout << ExceptionClass::GetLastErrorToString(GetLastError()).c_str() << endl;
		return FALSE;
	}
	return TRUE;
}

BOOL CWinManageInstrumentation::WMIIWbemClassObjectGet(WMIIENUMWBEMNEXTPARAM* _lpWMIIEnumWbemNextParam)			// 循环pEnumerator 获取结果	
{
	try
	{
		assert(m_pEnumerator);
		IWbemClassObject *pclsObj;
		ULONG uReturn = 0;

		while (m_pEnumerator)
		{
			HRESULT hres = m_pEnumerator->Next(_lpWMIIEnumWbemNextParam->_lTimeOut,
				_lpWMIIEnumWbemNextParam->_uCount,
				&pclsObj, &uReturn);
			if (FAILED(hres))
			{
				cout << "m_pEnumerator  next get failed. "
					<< "Error code = 0x"
					<< hex << hres << endl;
				m_pSvc->Release();
				m_pLoc->Release();
				CoUninitialize();
				return FALSE;               // Program has failed.
			}
			if (0 == uReturn)
			{
				break;
			}
			WMIPrint(pclsObj);					// 虚函数
		}
	}
	catch (ExceptionClass* e)
	{
		e->ReportError();
		m_pLoc->Release();
		CoUninitialize();
		return FALSE;
	}
	catch (...)
	{
		std::cout << ExceptionClass::GetLastErrorToString(GetLastError()).c_str() << endl;
		return FALSE;
	}
	return TRUE;
}

void CWinManageInstrumentation::WMIPrint(IWbemClassObject *pclsObj)
{
	VARIANT vtProp;

	// Get the value of the Name property
	HRESULT hres = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
	if (FAILED(hres))
	{
		return;
	}
	wcout << "Process Name : " << vtProp.bstrVal << endl;
	VariantClear(&vtProp);
}