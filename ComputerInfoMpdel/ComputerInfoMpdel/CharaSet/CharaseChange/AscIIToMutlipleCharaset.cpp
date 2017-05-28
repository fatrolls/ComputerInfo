//#include "..//..//stdafx.h"
#include "stdafx.h"

#include "AscIIToMutlipleCharaset.h"

CAscIIToMutlipleCharaset::CAscIIToMutlipleCharaset()
{
}
CAscIIToMutlipleCharaset::~CAscIIToMutlipleCharaset()
{
}

//************************************
// Method:    string2Wstring
// FullName:  CAscIIToMutlipleCharaset::string2Wstring
// Access:    public static
// Returns:   std::wstring
// Qualifier: string 转换 Wstring
// Parameter: string sToMatch
//************************************
wstring CAscIIToMutlipleCharaset::string2Wstring(string sToMatch)
{
	wstring wsToMatch;

#ifdef _A_WIN
	int iWLen = MultiByteToWideChar(CP_ACP, 0, sToMatch.c_str(), sToMatch.size(), 0, 0); // 计算转换后宽字符串的长度。（不包含字符串结束符）
	wchar_t *lpwsz = new wchar_t[iWLen + 1];
	MultiByteToWideChar(CP_ACP, 0, sToMatch.c_str(), sToMatch.size(), lpwsz, iWLen); // 正式转换。
	lpwsz[iWLen] = L'/0';
	wsToMatch = lpwsz;
	delete[]lpwsz;
#elif _A_LINUX
	setlocale(LC_CTYPE, ""); // 很重要，没有这一句，转换会失败。
	int iWLen = mbstowcs(nullptr, sToMatch.c_str(), sToMatch.length()); // 计算转换后宽字符串的长度。（不包含字符串结束符）
	wchar_t *lpwsz = new wchar_t[iWLen + 1];
	int i = mbstowcs(lpwsz, sToMatch.c_str(), sToMatch.length()); // 转换。（转换后的字符串有结束符）
	wsToMatch = lpwsz;
	delete[]lpwsz;
#endif
	return wsToMatch;
}