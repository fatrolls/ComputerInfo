#pragma once
//#include "..//..//stdafx.h"
#include "stdafx.h"

#include "MutlipleByteToAscII.h"
#include <stdlib.h>

CMutilpleCharasetToAscII::CMutilpleCharasetToAscII()
{
}

CMutilpleCharasetToAscII::~CMutilpleCharasetToAscII()
{
}

LPCTSTR CMutilpleCharasetToAscII::CtCharToLPCTSTR(const char* Ctchar)
{
	int num = MultiByteToWideChar(0, 0, Ctchar, -1, nullptr, 0);

	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, Ctchar, -1, wide, num);
	return (LPCTSTR)(LPSTR)wide;
	LPCTSTR str = nullptr;
	return 	str;
}

char* CMutilpleCharasetToAscII::CStringToCChar(TCHAR* str)
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)str, -1, NULL, NULL, 0, NULL);
	char *c = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)str, -1, c, dwNum, 0, NULL);
	return c;
}

std::string CMutilpleCharasetToAscII::CStringTostring(TCHAR* cstr)
{
	DWORD nLen = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)cstr, -1, nullptr, 0, nullptr, nullptr);
	if (nLen <= 0){
		std::string str("");
		return str;
	}
	char* pszDst = new char[nLen];
	if (nullptr == pszDst){
		std::string str("");
		return str;
	}
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)cstr, -1, pszDst, nLen, nullptr, nullptr);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;
	return strTemp;
}

//把宽字符串转换成字符串，输出使用
string CMutilpleCharasetToAscII::wstring2string(wstring sToMatch)
{
	string sResult;
#ifdef _A_WIN
	int iLen = WideCharToMultiByte(CP_ACP, NULL, sToMatch.c_str(), -1, NULL, 0, NULL, FALSE); // 计算转换后字符串的长度。（包含字符串结束符）
	char *lpsz = new char[iLen];
	WideCharToMultiByte(CP_OEMCP, NULL, sToMatch.c_str(), -1, lpsz, iLen, NULL, FALSE); // 正式转换。
	sResult.assign(lpsz, iLen - 1); // 对string对象进行赋值。
	delete[]lpsz;
#elif _A_LINUX
	int iLen = wcstombs(NULL, sToMatch.c_str(), 0); // 计算转换后字符串的长度。（不包含字符串结束符）
	char *lpsz = new char[iLen + 1];
	int i = wcstombs(lpsz, sToMatch.c_str(), iLen); // 转换。（没有结束符）
	lpsz[iLen] = '/0';
	sResult(lpsz);
	delete[]lpsz;
#endif
	return sResult;
}

std::wstring CMutilpleCharasetToAscII::Format(const wchar_t *format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	int count = _vsnwprintf_s(0, sizeof(0), _TRUNCATE, format, argptr);
	va_end(argptr);

	va_start(argptr, format);
	wchar_t* buf = (wchar_t*)malloc(count*sizeof(wchar_t));
	_vsnwprintf_s(buf, count, _TRUNCATE, format, argptr);
	va_end(argptr);

	std::wstring str(buf, count);
	free(buf);
	return str;
}