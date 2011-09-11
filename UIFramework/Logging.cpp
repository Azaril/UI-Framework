#include "Logging.h"

#ifdef _WINDOWS
#include <atlstr.h>
#else
#include <stdio.h>
#include <wchar.h>
#endif

namespace logging
{

void InternalDebugOutCallbackA(
    __in_z const CHAR* strText
    )
{
#ifdef _WINDOWS    
    OutputDebugStringA(strText);
#else
    printf("%s", strText);
#endif
}

void 
InternalDebugOutCallbackW(
    __in_z const WCHAR* strText
    )
{
#ifdef _WINDOWS
    OutputDebugStringW(strText);
#else
    wprintf(L"%ls", strText);
#endif
}

DebugOutCallbackFuncA g_DebugOutCallbackA = InternalDebugOutCallbackA;
DebugOutCallbackFuncW g_DebugOutCallbackW = InternalDebugOutCallbackW;

void 
SetDebugOutCallback(
    __in_opt DebugOutCallbackFuncA Func
    )
{
    g_DebugOutCallbackA = Func;
}

void 
SetDebugOutCallback(
    __in_opt DebugOutCallbackFuncW Func
    )
{
    g_DebugOutCallbackW = Func;
}

__out_opt DebugOutCallbackFuncA 
GetDebugOutCallbackA(
    )
{
    return g_DebugOutCallbackA;
}

__out_opt DebugOutCallbackFuncW 
GetDebugOutCallbackW(
    )
{
    return g_DebugOutCallbackW;
}

void 
InternalDebugOut(
    __in_z const CHAR* strText
    )
{
    if(g_DebugOutCallbackA != NULL)
    {
        g_DebugOutCallbackA(strText);
    }
}

void 
InternalDebugOut(
    __in_z const WCHAR* strText
    )
{
    if(g_DebugOutCallbackW != NULL)
    {
        g_DebugOutCallbackW(strText);
    }
}

void 
DebugOut(
    __in_z __format_string const CHAR* pFormat, 
    ...
    )
{
#ifdef _WINDOWS    
    CStringA FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

    InternalDebugOut(FormattedString);
#else
    CHAR Buffer[2048];
    
    va_list args;
    va_start(args, pFormat);
    
    vsnprintf(Buffer, sizeof(Buffer), pFormat, args);
    
    va_end(args);
    
    InternalDebugOut(Buffer);
#endif
}

void 
DebugOut(
    __in_z __format_string const WCHAR* pFormat, 
    ...
    )
{
#ifdef _WINDOWS    
    CStringW FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

    InternalDebugOut(FormattedString);
#else
    WCHAR Buffer[2048];
    
    va_list args;
    va_start(args, pFormat);
    
    vswprintf(Buffer, sizeof(Buffer), pFormat, args);
    
    va_end(args);
    
    InternalDebugOut(Buffer);
#endif
}

void ZoneOut(
    __in_z const CHAR* pZone, 
    __in_z __format_string const CHAR* pFormat, 
    ...)
{
#ifdef _WINDOWS    
    CStringA FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

    DebugOut("[ %s ] - %s", pZone, (LPCSTR)FormattedString);
#else
    CHAR Buffer[2048];
    
    va_list args;
    va_start(args, pFormat);
    
    vsnprintf(Buffer, sizeof(Buffer), pFormat, args);
    
    va_end(args);
    
    DebugOut("[ %s ] - %s", pZone, Buffer);
#endif
}

void
ZoneOut(
    __in_z const WCHAR* pZone, 
    __in_z __format_string const WCHAR* pFormat, 
    ...
    )
{
#ifdef _WINDOWS
    CStringW FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

	DebugOut(L"[ %ls ] - %ls", pZone, (LPCWSTR)FormattedString);
#else
    WCHAR Buffer[2048];
    
    va_list args;
    va_start(args, pFormat);
    
    vswprintf(Buffer, sizeof(Buffer), pFormat, args);
    
    va_end(args);
    
    DebugOut("[ %ls ] - %ls", pZone, Buffer);    
#endif
}

void 
ZoneLevelOut(
    UINT32 Level, 
    __in_z const CHAR* pZone, 
    __in_z __format_string const CHAR* pFormat, 
    ...
    )
{
    const CHAR* LoggingLevelText[] = 
    {   
        "", 
        "[ CRITICAL ] - ",
        "[     INFO ] - ",
        "[  VERBOSE ] - " 
    };
    
#ifdef _WINDOWS
    CStringA FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

	DebugOut("%s[ %s ] - %s", LoggingLevelText[Level], pZone, (LPCSTR)FormattedString);
#else
    CHAR Buffer[2048];
    
    va_list args;
    va_start(args, pFormat);
    
    vsnprintf(Buffer, sizeof(Buffer), pFormat, args);
    
    va_end(args);
    
    DebugOut("%s[ %s ] - %s", LoggingLevelText[Level], pZone, Buffer); 
#endif
}

void 
ZoneLevelOut(
    UINT32 Level, 
    __in_z const WCHAR* pZone, 
    __in_z __format_string const WCHAR* pFormat, 
    ...
    )
{
    const WCHAR* LoggingLevelText[] = 
    {   
        L"", 
        L"[ CRITICAL ] - ",
        L"[     INFO ] - ",
        L"[  VERBOSE ] - " 
    };
    
#ifdef _WINDOWS
    CStringW FormattedString;
    
    va_list args;
    va_start(args, pFormat);
    
    FormattedString.FormatV(pFormat, args);
    
    va_end(args);
    
	DebugOut(L"l%s[ %ls ] - %ls", LoggingLevelText[Level], pZone, (LPCSTR)FormattedString);
#else
    WCHAR Buffer[2048];
    
    va_list args;
    va_start(args, pFormat);
    
    vswprintf(Buffer, sizeof(Buffer), pFormat, args);
    
    va_end(args);
    
    DebugOut(L"%ls[ %ls ] - %ls", LoggingLevelText[Level], pZone, Buffer);     
#endif
}

}