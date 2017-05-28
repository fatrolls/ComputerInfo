//#include "..//..//stdafx.h"
#include "stdafx.h"
#include "CpuCache.h"
#pragma  once

#ifndef  CPUCACHE_CPP
#define  CPUCACHE_CPP

CCpuCache::CCpuCache()
{
}

CCpuCache::~CCpuCache()
{
}

int CCpuCache::GetCpuCache(map<string, int> &cache)
{
	typedef BOOL(WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
	LPFN_GLPI glpi;
	BOOL done = FALSE;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
	DWORD returnLength = 0;

	glpi = (LPFN_GLPI)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")),
		"GetLogicalProcessorInformation");
	if (NULL == glpi)
	{
		_tprintf(TEXT("\nGetLogicalProcessorInformation is not supported.\n"));
		return (1);
	}

	while (!done)
	{
		DWORD rc = glpi(buffer, &returnLength);

		if (FALSE == rc)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer)
				{
					free(buffer);
				}

				buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
				if (NULL == buffer)
				{
					_tprintf(TEXT("\nError: Allocation failure\n"));
					return (2);
				}
			}
			else
			{
				_tprintf(TEXT("\nError %d\n"), GetLastError());
				return (3);
			}
		}
		else
		{
			done = TRUE;
		}
	}

	ptr = buffer;
	DWORD cnt = returnLength / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);	// 计算SYSTEM_LOGICAL_PROCESSOR_INFORMATION结构体的数目
	for (DWORD i = 0; i < cnt; ++i)
	{
		if (RelationCache == ptr[i].Relationship)
		{
			if (ptr[i].Cache.Level == 1)
			{
				cache.insert(pair<string, int>("L1Cache", ptr[i].Cache.Size / 1024));
			}

			if (ptr[i].Cache.Level == 2)
			{
				cache.insert(pair<string, int>("L2Cache", ptr[i].Cache.Size / 1024));
			}
		}
	}

	free(buffer);

	return 0;
}

// test code
//map<string, int> cache;
//GetCpuCache(cache);
//printf("L1 Cache = %dKB\n", cache["L1Cache"]);
//printf("L2 Cache = %dKB", cache["L2Cache"]);

#endif