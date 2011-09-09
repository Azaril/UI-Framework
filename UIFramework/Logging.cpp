#include "Logging.h"

#ifdef _WINDOWS
#include <atlstr.h>
#endif

namespace logging
{

void InternalDebugOutCallbackA(
    __in_z const CHAR* strText
    )
{
#ifdef _WINDOWS    
    OutputDebugStringA(strText);
#endif
}

void 
InternalDebugOutCallbackW(
    __in_z const WCHAR* strText
    )
{
#ifdef _WINDOWS
    OutputDebugStringW(strText);
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

	CStringA ZoneFormat;

	ZoneFormat.Format("[ %s ] - %s", pZone, (LPCSTR)FormattedString);

    InternalDebugOut(ZoneFormat);
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

	CStringW ZoneFormat;

	ZoneFormat.Format(L"[ %s ] - %s", pZone, (LPCWSTR)FormattedString);

    InternalDebugOut(ZoneFormat);
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
#ifdef _WINDOWS
    CStringA FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

	CStringA ZoneFormat;

    CONST LPCSTR LoggingLevelText[] = { "", 
                                        "[ CRITICAL ] - ",
                                        "[     INFO ] - ",
                                        "[  VERBOSE ] - " };

	ZoneFormat.Format("%s[ %s ] - %s", LoggingLevelText[Level], pZone, (LPCSTR)FormattedString);

    InternalDebugOut(ZoneFormat);
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
#ifdef _WINDOWS
    CStringW FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

	CStringW ZoneFormat;

    CONST LPCWSTR LoggingLevelText[] = { L"", 
                                         L"[ CRITICAL ] - ",
                                         L"[     INFO ] - ",
                                         L"[  VERBOSE ] - " };

	ZoneFormat.Format(L"%s[ %s ] - %s", LoggingLevelText[Level], pZone, (LPCWSTR)FormattedString);

    InternalDebugOut(ZoneFormat);
#endif
}

}