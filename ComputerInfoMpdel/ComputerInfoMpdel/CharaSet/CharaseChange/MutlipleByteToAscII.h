#ifndef MutilpleByteToAscII_h__
#define MutilpleByteToAscII_h__

#pragma once
#ifdef CHARASETDLL
#define FACEAPI _declspec(dllexport)
#else
#define FACEAPI  _declspec(dllimport)
#endif

//����include��Ҫ�õ���ͷ�ļ�
#include<windows.h>
#include<cstdio>
#include <iostream>

using namespace  std;

class CMutilpleCharasetToAscII
{
public:
	CMutilpleCharasetToAscII();
	~CMutilpleCharasetToAscII();

public:
	static LPCTSTR CtCharToLPCTSTR(const char* Ctchar);			// const char -->LPCTSTR
	static std::string CStringTostring(TCHAR* cstr);			// CString --> string
	static char* CStringToCChar(TCHAR* str);					// CString -->ת��Ϊ const char*
	
	static string wstring2string(wstring sToMatch);				// �ѿ��ַ���ת�����ַ���
	static std::wstring Format(const wchar_t *format, ...);

	static LPCWSTR lpcharToLpcwstr();

	static string Lpctstr2string(LPCTSTR it){
		// lpctstr --> string
		CString cStr = (LPCTSTR)it;
		string s = CStringA(cStr);
		return s;
	}

protected:

private:
};

class auto_setlocate 
{
public:
	auto_setlocate()
	{
		setlocale(LC_ALL, "");
	}
};

#endif // MutilpleByteToAscII_h__
