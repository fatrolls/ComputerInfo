/***************************************************************************************************/
/*                              usb �豸ö��                                                         */
/*   ���Ը���GUID����ȡUSB�豸����Ϣ��������������SetupDiGetDeviceInstanceID������ȡDeviceID��				*/
/*	ͨ��SetupDiGetDeviceRegistryPropertyW��������ȡ������ԣ�ʾ���л�ȡ��Location path��location Info��		*/
/*	Ҳ���Ի�ȡ��������Ϣ�������Ϣ�������ﶨ��                                                              */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/***************************************************************************************************/

#pragma  once

#include <stdio.h>
#include <windows.h>
#include <SetupAPI.h>
#include <iostream>
#include <Usbiodef.h>
#include <Usbioctl.h>
#include <devguid.h>
#include <winioctl.h>
#include <INITGUID.h>
#include <Dbt.h>
#include <map>
#include <wchar.h>
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "setupapi.lib")

DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE,
	0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);
#define   GUID_CLASS_USB_DEVICE                   GUID_DEVINTERFACE_USB_DEVICE

DEFINE_GUID(GUID_DEVINTERFACE_USB_HUB,
	0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8);

#define  GUID_CLASS_USB_HUB GUID_DEVINTERFACE_USB_HUB
#define  COUT_OUTPUT

void getDevices(GUID lpguid)
{
	LPGUID lpGuid = (LPGUID)&lpguid;//
#ifndef COUT_OUTPUT
	std::cout << "LPGUID: " << lpguid << endl;
#endif
	int INTERFACE_DETAIL_SIZE = 1024;
	int nCount = 0;
	HDEVINFO info = SetupDiGetClassDevs(lpGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

	//enumerate device information
	DWORD required_size = 0;
	int i;
	SP_DEVINFO_DATA DeviceInfoData = { sizeof(DeviceInfoData) };

	DWORD DataT;
	DWORD buffersize = 1024;
	DWORD req_bufsize = 0;

	for (i = 0; SetupDiEnumDeviceInfo(info, i, &DeviceInfoData); i++)
	{
		WCHAR *did = new WCHAR[1024];
		memset(did, 0, 1024);
		WCHAR *locid = new WCHAR[1024];
		memset(locid, 0, 1024);
		WCHAR *locdesc = new WCHAR[1024];
		memset(locdesc, 0, 1024);
		//get device description information
		if (!SetupDiGetDeviceInstanceId(info, &DeviceInfoData, (PTSTR)did, buffersize, &req_bufsize))
		{
			continue;
		}
		printf("did: %ls\n", did);
		if (!SetupDiGetDeviceRegistryPropertyW(info, &DeviceInfoData, SPDRP_LOCATION_PATHS, &DataT, (LPBYTE)locid, buffersize, &req_bufsize))
		{
			continue;
		}
		printf("locid: %ls\n", locid);

		if (!SetupDiGetDeviceRegistryPropertyW(info, &DeviceInfoData, SPDRP_LOCATION_INFORMATION, &DataT, (LPBYTE)locdesc, buffersize, &req_bufsize))
		{
			continue;
		}
		printf("locdesc: %ls\n", locdesc);
	}

	SetupDiDestroyDeviceInfoList(info);
}