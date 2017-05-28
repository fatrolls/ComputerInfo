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
// Qualifier: string ת�� Wstring
// Parameter: string sToMatch
//************************************
wstring CAscIIToMutlipleCharaset::string2Wstring(string sToMatch)
{
	wstring wsToMatch;

#ifdef _A_WIN
	int iWLen = MultiByteToWideChar(CP_ACP, 0, sToMatch.c_str(), sToMatch.size(), 0, 0); // ����ת������ַ����ĳ��ȡ����������ַ�����������
	wchar_t *lpwsz = new wchar_t[iWLen + 1];
	MultiByteToWideChar(CP_ACP, 0, sToMatch.c_str(), sToMatch.size(), lpwsz, iWLen); // ��ʽת����
	lpwsz[iWLen] = L'/0';
	wsToMatch = lpwsz;
	delete[]lpwsz;
#elif _A_LINUX
	setlocale(LC_CTYPE, ""); // ����Ҫ��û����һ�䣬ת����ʧ�ܡ�
	int iWLen = mbstowcs(nullptr, sToMatch.c_str(), sToMatch.length()); // ����ת������ַ����ĳ��ȡ����������ַ�����������
	wchar_t *lpwsz = new wchar_t[iWLen + 1];
	int i = mbstowcs(lpwsz, sToMatch.c_str(), sToMatch.length()); // ת������ת������ַ����н�������
	wsToMatch = lpwsz;
	delete[]lpwsz;
#endif
	return wsToMatch;
}