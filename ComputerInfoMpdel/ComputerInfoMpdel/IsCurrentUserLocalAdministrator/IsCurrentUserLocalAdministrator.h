#pragma once

#include <windows.h>

#ifdef IsCurrentUserLocalAdministrator_lib
#define IsCurrentUserLocalAdministrator_API _declspec(dllexport)
#else
#define IsCurrentUserLocalAdministrator_API _declspec(dllimport)
#endif

#ifdef _UNICODE
BOOL IsCurrentUserLocalAdministrator(void);
#endif
