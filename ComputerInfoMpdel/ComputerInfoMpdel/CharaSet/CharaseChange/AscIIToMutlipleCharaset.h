#ifndef AscIIByteToMutilple_h__
#define AscIIByteToMutilple_h__

#pragma once
#ifdef AscIIByteToMutilple_DLL
#define AscIIByteToMutilple_API _declspec(dllexport)
#else
#define AscIIByteToMutilple_API  _declspec(dllimport)
#endif

//����include��Ҫ�õ���ͷ�ļ�
#include<windows.h>
#include<cstdio>
#include <iostream>

using namespace  std;

class CAscIIToMutlipleCharaset
{
public:
	CAscIIToMutlipleCharaset();
	virtual ~CAscIIToMutlipleCharaset();
	static wstring string2Wstring(string sToMatch);				// ���ַ���ת���ɿ��ַ���

};


#endif  // AscIIByteToMutilple_h__