#include "Logging.h"

#ifdef _WINDOWS
#include <atlstr.h>
#endif

namespace logging
{

void InternalDebugOutCallbackA(const CHAR* strText)
{
#ifdef _WINDOWS    
    OutputDebugStringA(strText);
#endif
}

void InternalDebugOutCallbackW(const WCHAR* strText)
{
#ifdef _WINDOWS
    OutputDebugStringW(strText);
#endif
}

DebugOutCallbackFuncA g_DebugOutCallbackA = InternalDebugOutCallbackA;
DebugOutCallbackFuncW g_DebugOutCallbackW = InternalDebugOutCallbackW;

void SetDebugOutCallback(DebugOutCallbackFuncA Func)
{
    g_DebugOutCallbackA = Func;
}

void SetDebugOutCallback(DebugOutCallbackFuncW Func)
{
    g_DebugOutCallbackW = Func;
}

DebugOutCallbackFuncA GetDebugOutCallbackA()
{
    return g_DebugOutCallbackA;
}

DebugOutCallbackFuncW GetDebugOutCallbackW()
{
    return g_DebugOutCallbackW;
}

void InternalDebugOut(const CHAR* strText)
{
    if(g_DebugOutCallbackA != NULL)
    {
        g_DebugOutCallbackA(strText);
    }
}

void InternalDebugOut(const WCHAR* strText)
{
    if(g_DebugOutCallbackW != NULL)
    {
        g_DebugOutCallbackW(strText);
    }
}

void DebugOut(const CHAR* pFormat, ...)
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
void DebugOut(const WCHAR* pFormat, ...)
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

void ZoneOut(const CHAR* pZone, const CHAR* pFormat, ...)
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

void ZoneOut(const WCHAR* pZone, const WCHAR* pFormat, ...)
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

void ZoneLevelOut(UINT32 Level, const CHAR* pZone, const CHAR* pFormat, ...)
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

void ZoneLevelOut(UINT32 Level, const WCHAR* pZone, const WCHAR* pFormat, ...)
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