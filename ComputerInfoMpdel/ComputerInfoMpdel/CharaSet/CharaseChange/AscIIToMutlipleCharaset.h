#ifndef AscIIByteToMutilple_h__
#define AscIIByteToMutilple_h__

#pragma once
#ifdef AscIIByteToMutilple_DLL
#define AscIIByteToMutilple_API _declspec(dllexport)
#else
#define AscIIByteToMutilple_API  _declspec(dllimport)
#endif

//可以include需要用到的头文件
#include<windows.h>
#include<cstdio>
#include <iostream>

using namespace  std;

class CAscIIToMutlipleCharaset
{
public:
	CAscIIToMutlipleCharaset();
	virtual ~CAscIIToMutlipleCharaset();
	static wstring string2Wstring(string sToMatch);				// 把字符串转换成宽字符串

};


#endif  // AscIIByteToMutilple_h__