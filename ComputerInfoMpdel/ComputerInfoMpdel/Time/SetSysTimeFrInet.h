#pragma  once

#include <windows.h>
#include <iostream>
#include <WinSock2.h>  
#pragma comment(lib, "ws2_32.lib") 



// DNS  ʱ�������
// time.nist.gov
// time.nist.gov
// time-nw.nist.gov
// time - a.nist.gov
// time - b.nist.gov
// �����ʼ��
class CInitSock
{
public:
	CInitSock(BYTE minorVer = 2, BYTE majorVer = 2)
	{
		WORD wVersionRequested = MAKEWORD(minorVer, majorVer);
		WSADATA wsaData;
		if (0 != ::WSAStartup(wVersionRequested, &wsaData))
		{
			exit(0);
		}

	}
	~CInitSock()
	{
		::WSACleanup();
	}
};

//���ݷ��ص�ʱ������ϵͳʱ��
void setTimeFromTP(ULONG ulTime)
{
	FILETIME ft;
	SYSTEMTIME st;

	//����׼ʱ��ת����windows�ļ�ʱ��  
	st.wYear = 1900;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);

	LONGLONG* pLLong = (LONGLONG*)&ft;
	*pLLong += (LONGLONG)10000000 * ulTime;
	FileTimeToSystemTime(&ft, &st);
	::SetSystemTime(&st);
}

void SetSysTimeFrmInet()
{
	CInitSock initSock;
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf("socket error...");
		return;
	}
	sockaddr_in servAddr = { 0 };
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = ::htons(37);
	servAddr.sin_addr.S_un.S_addr = ::inet_addr("129.6.15.28"); //����210.72.145.44���IP�� �����쳯�� ��TMD������ֵ  
	if (SOCKET_ERROR == ::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)))
	{
		printf("connect error..");
		return;
	}

	//����ʱ��  
	ULONG ulTime = 0;
	int nRecv = ::recv(s, (char*)&ulTime, sizeof(ulTime), 0);
	if (nRecv > 0)  //�յ�����  
	{
		ulTime = ::ntohl(ulTime);
		setTimeFromTP(ulTime);
	}
	else
	{
		printf("recv error...");
		return;
	}

	::closesocket(s);
	printf("**************����ʱ��ɹ�*****************\n");
}