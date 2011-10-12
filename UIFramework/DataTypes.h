#pragma once

#if _WINDOWS

#include <windows.h>

#include <float.h>

inline int 
isfinite(
	double Val
	)
{
    return _finite(Val);
}

#else

typedef int INT32;
typedef unsigned int UINT32;
typedef float FLOAT;
typedef wchar_t WCHAR;
typedef char CHAR;
typedef UINT32 BOOL;
typedef unsigned char BYTE;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef INT32 HRESULT;
typedef long int INT64;
typedef unsigned long int UINT64;

#ifndef NULL
#define NULL (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif  

#include <wchar.h>

#endif

#include <limits>

struct Nothing;