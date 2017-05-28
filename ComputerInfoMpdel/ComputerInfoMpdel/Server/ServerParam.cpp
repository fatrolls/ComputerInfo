#pragma  once

#include "stdafx.h"
#include "ServerParam.h"
#include <Windows.h>
#include <winsvc.h>


CServerParam::CServerParam()
{
}

CServerParam::~CServerParam()
{
}

void CServerParam::ShowServiceList(DWORD dwServiceType)
{
	SC_HANDLE  hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (NULL == hSCM)
	{
		printf("OpenSCManager Error!!");
		return;
	}

	DWORD dwBufSize = 512 * sizeof(ENUM_SERVICE_STATUS);
	DWORD dwByteNeeded, dwSericesReturned, lpResumeHandle = 0;
	ENUM_SERVICE_STATUS SerStatus[512] = { 0 };

	BOOL bRet = EnumServicesStatus(hSCM, dwServiceType,
		SERVICE_STATE_ALL,
		SerStatus, dwBufSize,
		&dwByteNeeded,
		&dwSericesReturned,
		&lpResumeHandle);
	if (false == bRet)
	{
		switch (GetLastError())
		{
		case ERROR_ACCESS_DENIED:
		{
															printf("The specified handle was not opened with SC_MANAGER_ENUMERATE_SERVICE access\n");
															break;
		}
		case ERROR_INVALID_HANDLE:
		{
															 printf("%d\n", ERROR_INVALID_HANDLE);
															 break;
		}
		case ERROR_INVALID_PARAMETER:
		{
																	printf("%d\n", ERROR_INVALID_PARAMETER);
																	break;
		}
		case ERROR_MORE_DATA:
		{
													printf("%d", ERROR_MORE_DATA);
													BOOL bRet = EnumServicesStatus(hSCM, dwServiceType,
														SERVICE_STATE_ALL,
														SerStatus, dwBufSize,
														&dwByteNeeded,
														&dwSericesReturned,
														&lpResumeHandle);
													if (false == bRet)
													{
														printf("EnumServicesStatus Error!\n");
														return;
													}
													else
													{
														// stl ¹ØÁªÈÝÆ÷
														for (DWORD i = 0; i < dwSericesReturned; i++)
														{
															printf("servname = %ls\n", SerStatus[i].lpServiceName);
															printf("displayname = %ls\n", SerStatus[i].lpDisplayName);
														//	::MessageBox(nullptr, SerStatus[i].lpServiceName, SerStatus[i].lpDisplayName, MB_OK);
															switch (SerStatus[i].ServiceStatus.dwCurrentState)
															{
															case SERVICE_PAUSED:
																printf("PAUSED\n");
																break;
															case SERVICE_START_PENDING:
																printf("starting\n");
																break;
															case SERVICE_STOP_PENDING:
																printf("stopping\n");
																break;
															case SERVICE_CONTINUE_PENDING:
																printf("continue is pending\n");
																break;
															case SERVICE_PAUSE_PENDING:
																printf("pause is pending\n");
																break;
															case SERVICE_STOPPED:
																printf("STOPPED\n");
																break;
															case SERVICE_RUNNING:
																printf("running\n");
															default:
																printf("other\n");
																break;
															}
														}
													}
													break;
		}
		default:
		{
						 printf("EnumServicesStatus\n");
		}
		}

		CloseServiceHandle(hSCM);
		return;
	}
	CloseServiceHandle(hSCM);
}

void CServerParam::OnBtnban(char szServiceName[MAXBYTE])
{
	// TODO: Add your control notification handler code here

	SC_HANDLE  hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (NULL == hSCM)
	{
		printf("OpenSCManager Error!\n");
		return;
	}
#ifdef UNICODE
	SC_HANDLE hscService = OpenService(hSCM, (LPCWSTR)szServiceName, SERVICE_ALL_ACCESS);
#else
	SC_HANDLE hscService = OpenService(hSCM, szServiceName, SERVICE_ALL_ACCESS);
#endif // !UNICODE

	SERVICE_STATUS ServiceStatus;

	BOOL bRet = ControlService(hscService, SERVICE_CONTROL_STOP, &ServiceStatus);

	CloseServiceHandle(hscService);
	CloseServiceHandle(hSCM);
}

void CServerParam::OnBtnstart(char szServiceName[MAXBYTE])
{
	// TODO: Add your control notification handler code here

	SC_HANDLE  hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (NULL == hSCM)
	{
		printf("OpenSCManager Error!\n");
		return;
	}

#ifdef UNICODE
	SC_HANDLE hscService = OpenService(hSCM, (LPCWSTR)szServiceName, SERVICE_ALL_ACCESS);
#else
	SC_HANDLE hscService = OpenService(hSCM, szServiceName, SERVICE_ALL_ACCESS);
#endif // !UNICODE

	BOOL bRet = StartService(hscService, 0, NULL);

	CloseServiceHandle(hscService);
	CloseServiceHandle(hSCM);
}

