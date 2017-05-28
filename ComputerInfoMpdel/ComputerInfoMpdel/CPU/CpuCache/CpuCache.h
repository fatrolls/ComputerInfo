#pragma  once

#ifndef CPUCACHE_H
#define CPUCACHE_H

#ifdef  CPUCACHE_DLL
#define CPUCACHEAPI _declspec (dllexport)
#else
#define CPUCACHEAPI _declspec (dllimport)
#endif

#include <Windows.h>
#include <malloc.h>    
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <map>

using namespace std;


#ifdef  __cplusplus
extern  "C"{
#endif

	class CCpuCache
	{
	public:
		CCpuCache();
		~CCpuCache();


	public:
		int GetCpuCache(map<string, int> &cache);
	private:

	};
	
#ifdef __cplusplus
}
#endif

#endif