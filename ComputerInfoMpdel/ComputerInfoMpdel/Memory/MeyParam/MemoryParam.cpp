#pragma  once


#include "stdafx.h"
#include "MemoryParam.h"


CMomeryParam::CMomeryParam()
{
	m_MemoryStatus = MyGlobalMemoryStatus();
}

CMomeryParam::~CMomeryParam()
{
}

MEMORYSTATUS CMomeryParam::MyGlobalMemoryStatus()
{
	//获得物理内存参数
	MEMORYSTATUS MemoryStatus;
	GlobalMemoryStatus(&MemoryStatus);
	return MemoryStatus;
}

DWORD CMomeryParam::GetMemoryLoad()
{
	return m_MemoryStatus.dwMemoryLoad;
}

DWORD CMomeryParam::GetAvailPhys()
{
	return m_MemoryStatus.dwAvailPhys;
}

DWORD CMomeryParam::TotalPageFile()
{
	return m_MemoryStatus.dwTotalPageFile;
}

DWORD CMomeryParam::GetAvailPageFile()
{
	return m_MemoryStatus.dwAvailPageFile;
}
DWORD CMomeryParam::GetTotalVirtual()
{
	return m_MemoryStatus.dwTotalVirtual;
}

DWORD CMomeryParam::GetAvailVirtual()
{
	return m_MemoryStatus.dwAvailVirtual;
}

DWORD CMomeryParam::GetTotalPhys()
{
	return m_MemoryStatus.dwTotalPhys;
}

DWORD CMomeryParam::ClearMemory()
{
//	WriteLog("..\\..\\..\\..\\Log\\Memory.txt", "ClearMemory");
	return 0;
}

