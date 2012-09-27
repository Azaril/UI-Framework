#pragma once

#include "EndianUtil.h"

#if defined(_WINDOWS)

#include <windows.h>

#include <float.h>

inline int 
isfinite(
	double Val
	)
{
    return _finite(Val);
}

#elif defined(_XBOX)

#include <wchar.h>
#include <float.h>

#define NOD3D
#include <xtl.h>

typedef int INT32;
typedef short INT16;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef float FLOAT;
typedef wchar_t WCHAR;
typedef char CHAR;
typedef unsigned char BYTE;
typedef long long LONGLONG;
typedef unsigned long ULONG;
typedef unsigned long long ULONGLONG;
typedef signed __int64 INT64;
typedef unsigned __int64 UINT64;
typedef ULONG DWORD;

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif  

inline int 
isfinite(
	double Val
	)
{
	return _finite(Val);
}

#else

typedef int INT32;
typedef short INT16;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef float FLOAT;
typedef wchar_t WCHAR;
typedef char CHAR;
typedef unsigned char BYTE;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef INT32 HRESULT;
typedef signed __int64 INT64;
typedef unsigned __int64 UINT64;

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