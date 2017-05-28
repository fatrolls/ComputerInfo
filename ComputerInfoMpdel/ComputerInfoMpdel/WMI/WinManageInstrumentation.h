/***************************************************************************************************/
/*                                                                                                  */
/*            WMI 功能进行类封装                                                                      */
/*          1、使用 TestCode                                                                         */
/*             重载虚函数WMIPrint																		*/
/*			   WMIExeQuery 的查询语句                                                                 */
/*          2、CIID  <----> SCLID                                                                   */
/*                                                                                                  */
/*                                                                                                  */
/***************************************************************************************************/

#pragma  once



#include <Wbemidl.h>

#define _WIN32_DCOM
#include <iostream>
#include <comutil.h>
using namespace std;
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsupp.lib")




//	Registers security and sets the default security values for the process.
typedef struct WMICOINITSECURITYPARAM
{
	PSECURITY_DESCRIPTOR			_pSecDesc;				// The access permissions that a server will use to receive calls.
	LONG							_cAuthSvc;				// The count of entries in the asAuthSvc parameter.
	SOLE_AUTHENTICATION_SERVICE     *_asAuthSvc;			// An array of authentication services that a server is willing to use to receive a call.
	void							*_pReserved1;			// This parameter is reserved and must be NULL.
	DWORD							_dwAuthnLevel;			// The default impersonation level for proxies.
	DWORD							_dwImpLevel;			// The default authentication level for the process. Both servers and clients use this parameter when they call CoInitializeSecurity.
	void						    *_pAuthList;			// A pointer to SOLE_AUTHENTICATION_LIST, which is an array of SOLE_AUTHENTICATION_INFO structures
	DWORD							_dwCapabilities;		// Additional capabilities of the client or server, specified by setting one or more EOLE_AUTHENTICATION_CAPABILITIES values.
	void							*_pReserved3;			// This parameter is reserved and must be NULL.

	// 初始化
	WMICOINITSECURITYPARAM()
	{
		_pSecDesc = NULL;
		_cAuthSvc = -1;      // COM negotiates service                  
		_asAuthSvc = NULL;   // Authentication services
		_pReserved1 = NULL;   // Reserved
		_dwAuthnLevel = RPC_C_AUTHN_LEVEL_DEFAULT;    // authentication
		_dwImpLevel = RPC_C_IMP_LEVEL_IMPERSONATE;  // Impersonation
		_pAuthList = NULL;            // Authentication info 
		_dwCapabilities = EOAC_NONE;     // Additional capabilities
		_pReserved3 = NULL;          // Reserved
	}
} _WMICoInitSecurityParam, *_lpWMICoInitSecurityParam;

// Creates a single uninitialized object of the class associated with a specified CLSID.
//// riid and ppv;
//typedef struct WMICOCREATEINSTANCEPARAM
//{
//	_In_  IID  rclsid;									// The CLSID associated with the data and code that will be used to create the object.
//	_In_  LPUNKNOWN pUnkOuter;								// If NULL, indicates that the object is not being created as part of an aggregate. If non-NULL, pointer to the aggregate object's IUnknown interface (the controlling IUnknown).
//	_In_  DWORD     dwClsContext;							// Context in which the code that manages the newly created object will run. The values are taken from the enumeration CLSCTX.
//	_In_  REFIID    riid;									// A reference to the identifier of the interface to be used to communicate with the object.
//	_Out_ LPVOID    *ppv;									// Address of pointer variable that receives the interface pointer requested in riid. Upon successful return, *ppv contains the requested interface pointer. Upon failure, *ppv contains NULL.
//	//WMICOCREATEINSTANCEPARAM()
//	//{
//		rclsid = CLSID_WbemLocator;
//	//		pUnkOuter = 0;
//	//		dwClsContext = CLSCTX_INPROC_SERVER;
//	//
//	//}
//} _WMICOCreateInstanceParam, *_lpWMICOCreateInstanceParam;


// ConnectServer  Param
// 输出参数_Out_       IWbemServices **_ppNamespace;	
// _strNetworkResource 需求初始化
typedef struct  WMICONNECTSERVERPARAM
{
	_In_   BSTR          _strNetworkResourceConn;		    // Pointer to a valid BSTR that contains the object path of the correct WMI namespace.
	_In_   BSTR          _strUser;						// Pointer to a valid BSTR, which contains the user name you need for a connection. StrUserName = SysAllocString(L"Domain\\UserName");
	_In_   BSTR          _strPassword;					// Pointer to a valid BSTR that contains the password you need for a connection. A NULL value indicates the current security context. A blank string "" specifies a valid zero-length password.
	_In_   BSTR          _strLocale;					// If NULL, the current locale is used. If not NULL, this parameter must be a valid BSTR,  Windows 7:  If an invalid locale is specified, then the default locale of the server is used unless there is a server-supported locale provided by the user application.
	_In_   LONG			 _lSecurityFlags;				// Long value used to pass flag values to ConnectServer.  WBEM_FLAG_CONNECT_REPOSITORY_ONLY (64 (0x40))  WBEM_FLAG_CONNECT_USE_MAX_WAIT (128 (0x80))
	_In_   BSTR          _strAuthority;				// This parameter contains the name of the domain of the user to authenticate.
	_In_        IWbemContext  *_pCtx;						// Typically, this is NULL. Otherwise, this is a pointer to an IWbemContext object required by one or more dynamic class providers.
	_Out_       IWbemServices **_ppNamespace;				// Receives a pointer to an IWbemServices object bound to the specified namespace.
	WMICONNECTSERVERPARAM()
	{
		_strNetworkResourceConn = _bstr_t("ROOT\\CIMV2");
		_strUser = nullptr;
		_strPassword = nullptr;
		_strLocale = 0;
		_lSecurityFlags = 0;
		_strAuthority = 0;
		_pCtx = nullptr;
	}
} _WMIConnectServerParam, *_lpWMIConnectServerParam;

// Sets the authentication information that will be used to make calls on the specified proxy.
// _pProxy 代理 m_pSvc
typedef struct WMICOSETPROXYBLANKETPARAM
{
	_In_     IUnknown                 *_pProxy;				// The proxy to be set.
	_In_     DWORD                    _dwAuthnSvc;			// The authentication service to be used. For a list of possible values, see Authentication Service Constants.
	_In_     DWORD                    _dwAuthzSvc;			// The authorization service to be used. For a list of possible values, see Authorization Constants. If RPC_C_AUTHZ_DEFAULT is specified
	_In_opt_ OLECHAR                  *_pServerPrincName;   // The server principal name to be used with the authentication service.
	_In_     DWORD                    _dwAuthnLevel;		// The authentication level to be used. For a list of possible values, see Authentication Level Constants.
	_In_     DWORD                    _dwImpLevel;			// The impersonation level to be used. For a list of possible values, see Impersonation Level Constants.
	_In_opt_ RPC_AUTH_IDENTITY_HANDLE _pAuthInfo;			// A pointer to an RPC_AUTH_IDENTITY_HANDLE value that establishes the identity of the client. The format of the structure referred to by the handle depends on the provider of the authentication service.
	_In_     DWORD                    _dwCapabilities;	    // The capabilities of this proxy. For a list of possible values, see the EOLE_AUTHENTICATION_CAPABILITIES enumeration.
	WMICOSETPROXYBLANKETPARAM()
	{
	//	_pProxy = ;						// 代理服务器
		_dwAuthnSvc = RPC_C_AUTHN_WINNT;
		_dwAuthzSvc = RPC_C_AUTHZ_NONE;
		_pServerPrincName = NULL;
		_dwAuthnLevel = RPC_C_AUTHN_LEVEL_CALL;
		_dwImpLevel = RPC_C_IMP_LEVEL_IMPERSONATE;
		_pAuthInfo = NULL;
		_dwCapabilities = EOAC_NONE;
	}
} _WMICoSetProxyBlanketParam, *_lpWMICoSetProxyBlanketParam;

typedef struct  WMIQUERYPARAM
{
	_In_    BSTR                 _strQueryLanguage;	// Valid BSTR that contains one of the query languages supported by Windows Management. This must be "WQL", the acronym for WMI Query Language.
	_In_    BSTR                 _strQuery;			// Valid BSTR that contains the text of the query. This parameter cannot be NULL. For more information on building WMI query strings, see Querying with WQL and the WQL reference.
	_In_         LONG                 _lFlags;				// The following flags affect the behavior of this method. The suggested value for this parameter is WBEM_FLAG_RETURN_IMMEDIATELY and WBEM_FLAG_FORWARD_ONLY for best performance.
	_In_         IWbemContext         *_pCtx;				// Typically NULL. Otherwise, this is a pointer to an IWbemContext object that can be used by the provider that is providing the requested classes or instances.
	_Out_       IEnumWbemClassObject  **_ppEnum;			// If no error occurs, this receives the enumerator that allows the caller to retrieve the instances in the result set of the query. I
	WMIQUERYPARAM()
	{
		//_strQueryLanguage = _bstr_t("WQL");
		//_strQuery = _bstr_t("");
		_lFlags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;
		_pCtx = nullptr;
	}
} _WMIQueryParam, *_lpWMIQueryParam;

// Use the IEnumWbemClassObject::Next method to get one or more objects starting at the current position in an enumeration.
typedef struct WMIIENUMWBEMNEXTPARAM{
	_In_  LONG				_lTimeOut;						// Specifies the maximum amount of time in milliseconds that the call blocks before returning.WBEM_INFINITE (0xFFFFFFFF), If you use the value 0 (WBEM_NO_WAIT)
	_In_  ULONG				_uCount;						// Number of requested objects.
	_Out_ IWbemClassObject  **_ppObjects;					// Pointer to enough storage to hold the number of IWbemClassObject interface pointers specified by uCount.
	_Out_ ULONG             *_puReturned;					// Pointer to a ULONG that receives the number of objects returned.
	WMIIENUMWBEMNEXTPARAM()
	{
		_lTimeOut = WBEM_INFINITE;
		_uCount = 1;
	}
} _WMIIEnumWbemNextParam, *_lpWMIIEnumWbemNextParam;

typedef struct WMIIWBEMCLASSGETPARAM{
	_In_       LPCWSTR			_wszName;					// Name of the desired property. It is treated as read-only.
	_In_	   LONG				_lFlags;					// Reserved. This parameter must be 0 (zero).
	_Out_opt_ VARIANT			*_pVal;						// When successful, this parameter is assigned the correct type and value for the qualifier, and the VariantInit function is called on pVal.
	_Out_opt_ CIMTYPE			*_pvtType;					// Can be NULL. If it is not NULL, it receives the CIM type of the property,
	_Out_opt_ LONG				*_plFlavor;					// Can be NULL. If not NULL, the LONG value pointed to receives information about the origin of the property. For more information, see Qualifier Flavors and WBEM_FLAVOR_TYPE.
	
} _WMIIWbemClassGetParam, *_lpWMIIWbemClassGetParam;


class CWinManageInstrumentation
{
public:
	CWinManageInstrumentation();
	~CWinManageInstrumentation();

public:

	BOOL WMICoInitializeEx(DWORD dwCoInit = COINIT_MULTITHREADED);								// COM 库初始化
	BOOL WMICoInitializeSecurity(WMICOINITSECURITYPARAM *_lpWMICoInitSecurityParam);            // 设置寄存器安全和进程安全等级
	//BOOL WMICoCreateInstance(WMICOCREATEINSTANCEPARAM* _lpWMICOCreateInstanceParam);			// 创建实例
	BOOL WMICoCreateInstance();			// 创建实例
	BOOL WMIConnectServer(WMICONNECTSERVERPARAM *_lpWMIConnectServerParam);						// 连接服务器
	BOOL WMICoSetProxyBlanket(WMICOSETPROXYBLANKETPARAM* _lpWMICoSetProxyBlanketParam);			// 代理服务器
	BOOL WMIExecQuery(BSTR  _strQueryLanguage, BSTR _strQuery,WMIQUERYPARAM* _lpWMIQueryParam);											// 查询 定义查询类型
	BOOL WMIIWbemClassObjectGet(WMIIENUMWBEMNEXTPARAM* _lpWMIIEnumWbemNextParam);				// 循环pEnumerator 获取结果														// 
	virtual void WMIPrint(IWbemClassObject *pclsObj) ;													// 打印信息

private:

	IWbemLocator *m_pLoc;
	IWbemServices *m_pSvc;
	IEnumWbemClassObject* m_pEnumerator;
};






// testcode 
// 重载虚函数WMIPrint
// WMIExeQuery 的查询语句
//int _tmain(int argc, _TCHAR* argv[])
//{
//	WMICOINITSECURITYPARAM* _lpWMICoInitSecurityParam = new WMICOINITSECURITYPARAM();
//	WMICONNECTSERVERPARAM* _lpWMIConnectServerParam = new WMICONNECTSERVERPARAM();
//	_lpWMIConnectServerParam->_strNetworkResourceConn = _bstr_t("ROOT\\CIMV2");
//
//	WMICOSETPROXYBLANKETPARAM* _lpWMICoSetProxyBlanketParam = new WMICOSETPROXYBLANKETPARAM();
//	WMIQUERYPARAM* _lpWMIQueryParam = new WMIQUERYPARAM();
//	//_lpWMIQueryParam->_strQueryLanguage = _bstr_t("WQL");
//	//_lpWMIQueryParam->_strQuery = _bstr_t("SELECT * FROM Win32_SystemServices");
//	WMIIENUMWBEMNEXTPARAM* _lpWMIIEnumWbemNextParam = new WMIIENUMWBEMNEXTPARAM();
//
//	CWinManageInstrumentation* m_wmi = new CWinManageInstrumentation();
//	m_wmi->WMICoInitializeEx();
//	m_wmi->WMICoInitializeSecurity(_lpWMICoInitSecurityParam);
//	m_wmi->WMICoCreateInstance();
//	m_wmi->WMIConnectServer(_lpWMIConnectServerParam);
//	m_wmi->WMICoSetProxyBlanket(_lpWMICoSetProxyBlanketParam);
//	m_wmi->WMIExecQuery(_bstr_t("WQL"), _bstr_t("SELECT * FROM Win32_Process"), _lpWMIQueryParam);
//	m_wmi->WMIIWbemClassObjectGet(_lpWMIIEnumWbemNextParam);
//	return 0;
//}






