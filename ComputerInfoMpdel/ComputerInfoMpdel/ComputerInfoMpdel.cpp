// ComputerInfoMpdel.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "ComputerInfoMpdel.h"

#include <windows.h>
#include "CPU/CPUParam.h"
#include "CPU//CpuCache//CpuCache.h"
#include "SysVersion/SysVerInfo.h"

#include "Password//GetPassword.h"
#include "Password/process.h"
#include "Server/ServerParam.h"
#include "Memory/MeyParam/MemoryParam.h"
#include "IsCurrentUserLocalAdministrator/IsCurrentUserLocalAdministrator.h"
#include "ProcessInfomation/psapiAnys/procanalyze.h"
#include "Register/InstallPrgmReg/InstallPrgmReg.h"
#include "CharaSet/CharaseChange/MutlipleByteToAscII.h"
#include "Time/SetSysTimeFrInet.h"
#include "Time\Clock\ClockTikt.h"
#include "Time/SimpleTimer/SimpleTimer.h"
#include "CPU/testccpuid.h"

#include "USB/USBDevice.h"
#include "Register/32Enum64Reg/32Enum64Reg.h"
#include "WMI/WinManageInstrumentation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一的应用程序对象

void Systeminfo();
void GetSystemPasswd();
void Serverinfo();
void Memory();
BOOL IsCurrentAdmini();
void Process();
void GetInstallPrgmReg();
void PrintSTLInfo(std::vector<LPCTSTR> lpszstr);
void SetSysTime();
void SimpleTimerTest();
void EnumUsbDevice();		// 枚举usb驱动设备
void Reg32T64();			// 32位程序枚举64位系统注册表子项，含关闭重定向和提权
void WMITest();				// WMI test

SystemInfo* sysInfo = new SystemInfo();
CServerParam* g_serverparam = new CServerParam();
CMomeryParam* g_momeryparam = new CMomeryParam();
ProcAnalyze* g_proanaly = new ProcAnalyze();
CInstallPgmReg* g_installpgmReg = new CInstallPgmReg();

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  更改错误代码以符合您的需要
			_tprintf(_T("错误:  MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  在此处为应用程序的行为编写代码。
			// CPU HardInfo

			CClockTikt * m_clocktikt = new CClockTikt();
			CMicroSecondClockTikt *m_microsendclock = new CMicroSecondClockTikt();
			//开始时间
			clock_t start = m_clocktikt->GetStartClock();
			time_t t_start = m_microsendclock->GetStartClock();

			printf("------------CPU HardInfo----------------\n");
			m_IsNT = IsNT();
			InitRing0Instance();
			Refresh();
			UnstllRing0Instance();

			// CPU Cache
			printf("------------CPU Cache----------------\n");
			CCpuCache m_cpucache;
			map<string, int> cache;
			m_cpucache.GetCpuCache(cache);
			printf("L1 Cache = %dKB\n", cache["L1Cache"]);
			printf("L2 Cache = %dKB\n", cache["L2Cache"]);

			// cpu hardinfo
			testcode();

			// system info
			Systeminfo();
			delete sysInfo;
			// 调用 getPasswd
			//GetSystemPasswd();				//x64 x86 编写
			// server info
			Serverinfo();
			delete g_serverparam;

			// memory
			Memory();
			delete g_momeryparam;

			// 调用者是本地机器上的管理员
			std::cout << "-------------judgement current user is administrator--------------------" << endl;
			if (IsCurrentAdmini())
			{
				std::cout << "Current user are administrator group!" << endl;
			}
			else
			{
				std::cout << "Current user not administrator group!" << endl;
			}
			// 进程信息
			//Process();

			// 获取已安装程序和更新补丁
			//GetInstallPrgmReg();
			delete g_installpgmReg;

			// 设置系统时间
			SetSysTime();

			// 简单计时器
			//SimpleTimerTest();

			// 枚举设备驱动
			EnumUsbDevice();

			// X86 读取X64注册表
			//Reg32T64();
			
			// WMI
			WMITest();


			// 结束时间
			clock_t end = m_clocktikt->GetEndClock();
			m_clocktikt->PrintTakenClock(start, end);
			delete m_clocktikt;

			time_t t_end = m_microsendclock->GetEndClock();
			m_microsendclock->PrintTakenClock(t_start, t_end);
			delete m_microsendclock;
		}
	}
	else
	{
		// TODO:  更改错误代码以符合您的需要
		_tprintf(_T("错误:  GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

void Systeminfo()
{
	printf("------------opeartor system version information-------------------------\n");

	// SystemInfo sysInfo;
	TCHAR szServicePack[128] = { 0 };

	switch (sysInfo->GetWindowsVersion())
	{
	case Windows:                 cout << "Windows" << endl; break;
	case Windows32s:              cout << "Windows 32s" << endl; break;
	case Windows95:               cout << "Windows 95" << endl; break;
	case Windows95OSR2:           cout << "Windows 95 SR2" << endl; break;
	case Windows98:               cout << "Windows 98" << endl; break;
	case Windows98SE:             cout << "Windows 98 SE" << endl; break;
	case WindowsMillennium:       cout << "Windows Me" << endl; break;
	case WindowsNT351:            cout << "Windows NT 3.51" << endl; break;
	case WindowsNT40:             cout << "Windows NT 4.0" << endl; break;
	case WindowsNT40Server:       cout << "Windows NT 4.0 Server" << endl; break;
	case Windows2000:             cout << "Windows 2000" << endl; break;
	case WindowsXP:               cout << "Windows XP" << endl; break;
	case WindowsXPProfessionalx64:cout << "Windows XP Professional x64" << endl; break;
	case WindowsHomeServer:       cout << "Windows Home Server" << endl; break;
	case WindowsServer2003:       cout << "Windows Server 2003" << endl; break;
	case WindowsServer2003R2:     cout << "Windows Server 2003 R2" << endl; break;
	case WindowsVista:            cout << "Windows Vista" << endl; break;
	case WindowsServer2008:       cout << "Windows Server 2008" << endl; break;
	case WindowsServer2008R2:     cout << "Windows Server 2008 R2" << endl; break;
	case Windows7:                cout << "Windows 7" << endl; break;
	case Windows8:                cout << "Windows 8" << endl; break;
	case  Windows10:              cout << "Windows 10" << endl; break;
	}

	switch (sysInfo->GetWindowsEdition())
	{
	case EditionUnknown:    cout << "Edition unknown Edition" << endl; break;
	case Workstation:       cout << "Workstation Edition" << endl; break;
	case Server:            cout << "Server Edition" << endl; break;
	case AdvancedServer:    cout << "Advanced Server Edition" << endl; break;
	case Home:              cout << "Home Edition" << endl; break;
	case Ultimate:          cout << "Ultimate Edition" << endl; break;
	case HomeBasic:         cout << "Home Basic Edition" << endl; break;
	case HomePremium:       cout << "Home Premium Edition" << endl; break;
	case Enterprise:        cout << "Enterprise Edition" << endl; break;
	case HomeBasic_N:       cout << "Home Basic N Edition" << endl; break;
	case Business:          cout << "Business Edition" << endl; break;
	case StandardServer:    cout << "Standard Server Edition" << endl; break;
	case EnterpriseServerCore: cout << "Enterprise Server Core Edition" << endl; break;
	case EnterpriseServerIA64: cout << "Enterprise Server IA64 Edition" << endl; break;
	case Business_N:           cout << "Business N Edition" << endl; break;
	case WebServer:            cout << "Web Server Edition" << endl; break;
	case ClusterServer:        cout << "Cluster Server Edition" << endl; break;
	case HomeServer:           cout << "Home Server Edition" << endl; break;
		// etc. etc.
	}

	cout << "Platform type: ";
	if (sysInfo->IsNTPlatform()) cout << "NT" << endl;
	else if (sysInfo->IsWindowsPlatform()) cout << "Windows" << endl;
	else if (sysInfo->IsWin32sPlatform()) cout << "Win32s" << endl;
	else cout << "Unknown" << endl;

	cout << "Major version: " << sysInfo->GetMajorVersion() << endl;
	cout << "Minor version: " << sysInfo->GetMinorVersion() << endl;
	cout << "Build number: " << sysInfo->GetBuildNumber() << endl;

	//	sysInfo->GetServiceInfoPack(szServicePack);  // m_ServicePack----->szServicePack
	//    cout << "Service Pack info: " << szServicePack << endl;

	cout << "Service Pack info: " << sysInfo->m_szServicePack << endl;

	cout << "32-bit platform: " << std::boolalpha << sysInfo->Is32bitPlatform() << endl;
	cout << "64-bit platform: " << std::boolalpha << sysInfo->Is64bitPlatform() << endl;

	delete sysInfo;
	sysInfo = nullptr;
}

void GetSystemPasswd()
{
	printf("------------getSystemPasswd----------------\n");
	system("color 0a");
	system("chcp 936");
	wcout.imbue(locale("chs"));
	Privilege(SE_DEBUG_NAME, TRUE);
	if (getVersion(&GLOB_Version))
	{
		loadLsaSrv();
		getLogonData();
	}
	else
		cout << "get Windows Version ERROR" << endl;
	wait();
}

void Serverinfo()
{
	printf("--------------server-------------------\n");
	g_serverparam->ShowServiceList(SERVICE_WIN32);
	delete g_serverparam;
	g_serverparam = nullptr;
}

void Memory()
{
	printf("---------------memory test-------------------\n");

	printf("%d\n", g_momeryparam->GetMemoryLoad());
	printf("%dK\n", g_momeryparam->GetAvailPageFile() / 1024);
	printf("%dK\n", g_momeryparam->GetAvailVirtual() / 1024);
	printf("%dK\n", g_momeryparam->GetAvailPhys() / 1024);
	printf("%dK\n", g_momeryparam->GetTotalVirtual() / 1024);
	printf("%dK\n", g_momeryparam->GetTotalPhys() / 1024);

	delete g_momeryparam;
	g_momeryparam = nullptr;
}

// 调用者是本地机器上的管理员
BOOL IsCurrentAdmini()
{
	return IsCurrentUserLocalAdministrator();
}
// 进程
void Process()
{
	std::cout << "-------------------process inforamtion---------------" << endl;
	g_proanaly->ListProcess();

	delete g_proanaly;
	g_proanaly = nullptr;
}

// 获取安装程序：注册表防晒霜
void GetInstallPrgmReg()
{
	std::cout << "-------------------install program & Update package inforamtion---------------" << endl;
	// 程序名
	std::vector<LPCTSTR> lpszSoftName;
	g_installpgmReg->GetSoftName(lpszSoftName);
	PrintSTLInfo(lpszSoftName);

	std::cout << "-------------------install program & Update package inforamtion---------------" << endl;
	std::vector<LPCTSTR> lpszsoftverison;
	g_installpgmReg->GetSoftVersion(lpszsoftverison);
	PrintSTLInfo(lpszsoftverison);
	std::cout << "-------------------install program & Update package inforamtion---------------" << endl;
	std::vector<LPCTSTR>lpszInstallLocation;
	g_installpgmReg->GetInstallLocation(lpszInstallLocation);
	PrintSTLInfo(lpszInstallLocation);
	std::cout << "-------------------install program & Update package inforamtion---------------" << endl;
	std::vector<LPCTSTR>lpszPublisher;
	g_installpgmReg->GetPublisher(lpszPublisher);
	PrintSTLInfo(lpszPublisher);
	std::cout << "-------------------install program & Update package inforamtion---------------" << endl;
	std::vector<LPCTSTR> lpszMainProPath;
	g_installpgmReg->GetMainProPath(lpszMainProPath);
	PrintSTLInfo(lpszMainProPath);
	std::cout << "-------------------install program &lpszSystemPatchesName& Update package inforamtion---------------" << endl;
	std::vector<LPCTSTR> lpszSystemPatchesName;
	g_installpgmReg->GetSystemPatchesName(lpszSystemPatchesName);
	PrintSTLInfo(lpszSystemPatchesName);
}

// 数据打印
void PrintSTLInfo(std::vector<LPCTSTR> lpszstr)
{
	std::vector<LPCTSTR>::iterator it = lpszstr.begin();
	for (; it != lpszstr.end(); it++)
	{
		// lpctstr --> string
		string s = CMutilpleCharasetToAscII::Lpctstr2string(*it);
		cout << s.c_str() << endl;
	}
}

// 设置系统时间
void SetSysTime()
{
	SetSysTimeFrmInet();
}

// 简单计时器
void SimpleTimerTest()
{
	CSimpleTimer t;
	char ch;
	hidden();//隐藏光标
	system("color 02");
	gotoxy(35, 12);
	cout << "00:00:00";
	gotoxy(20, 18);
	cout << "按a开始，按空格暂停，按s停止";
	while (1)
	{
		if (kbhit())
		{
			ch = getch();
			switch (ch)
			{
			case 'a':t.Start(); break;
			case 's':t.Stop(); break;
			case ' ':t.Pause(); break;
			default:break;
			}
		}
		if (!t.isStop() && !t.isPause())
		{
			t.show();
		}
	}
}

// 枚举usb驱动设备
void EnumUsbDevice()
{
	// GUID_CLASS_USB_HUB  GUID_CLASS_USB_DEVICE
	std::cout << " -------------------USB Device---------------------------" << endl;
	getDevices(GUID_CLASS_USB_DEVICE);
}

// 32位程序枚举64位系统注册表子项，含关闭重定向和提权
void Reg32T64()
{
	std::cout << "-------------------Reg32T64---------------" << endl;
	EnumReg32To64();
}

// WMI test
void WMITest()
{
	WMICOINITSECURITYPARAM* _lpWMICoInitSecurityParam = new WMICOINITSECURITYPARAM();
		WMICONNECTSERVERPARAM* _lpWMIConnectServerParam = new WMICONNECTSERVERPARAM();
		_lpWMIConnectServerParam->_strNetworkResourceConn = _bstr_t("ROOT\\CIMV2");
	
		WMICOSETPROXYBLANKETPARAM* _lpWMICoSetProxyBlanketParam = new WMICOSETPROXYBLANKETPARAM();
		WMIQUERYPARAM* _lpWMIQueryParam = new WMIQUERYPARAM();
		//_lpWMIQueryParam->_strQueryLanguage = _bstr_t("WQL");
		//_lpWMIQueryParam->_strQuery = _bstr_t("SELECT * FROM Win32_SystemServices");
		WMIIENUMWBEMNEXTPARAM* _lpWMIIEnumWbemNextParam = new WMIIENUMWBEMNEXTPARAM();
	
		CWinManageInstrumentation* m_wmi = new CWinManageInstrumentation();
		m_wmi->WMICoInitializeEx();
		m_wmi->WMICoInitializeSecurity(_lpWMICoInitSecurityParam);
		m_wmi->WMICoCreateInstance();
		m_wmi->WMIConnectServer(_lpWMIConnectServerParam);
		m_wmi->WMICoSetProxyBlanket(_lpWMICoSetProxyBlanketParam);
		m_wmi->WMIExecQuery(_bstr_t("WQL"), _bstr_t("SELECT * FROM Win32_Process"), _lpWMIQueryParam);
		m_wmi->WMIIWbemClassObjectGet(_lpWMIIEnumWbemNextParam);
		delete m_wmi;
		delete _lpWMICoInitSecurityParam;
		delete _lpWMIConnectServerParam;
		delete _lpWMICoSetProxyBlanketParam;
		delete _lpWMIQueryParam;
		delete _lpWMIIEnumWbemNextParam;
}