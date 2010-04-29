#include "StdAfx.h"
#include "Logging.h"

VOID InternalDebugOutCallbackA(LPCSTR strText)
{
    OutputDebugStringA(strText);
}

VOID InternalDebugOutCallbackW(LPCWSTR strText)
{
    OutputDebugStringW(strText);
}

DebugOutCallbackFuncA g_DebugOutCallbackA = InternalDebugOutCallbackA;
DebugOutCallbackFuncW g_DebugOutCallbackW = InternalDebugOutCallbackW;

VOID SetDebugOutCallback(DebugOutCallbackFuncA Func)
{
    g_DebugOutCallbackA = Func;
}

VOID SetDebugOutCallback(DebugOutCallbackFuncW Func)
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

VOID InternalDebugOut(LPCSTR strText)
{
    if(g_DebugOutCallbackA != NULL)
    {
        g_DebugOutCallbackA(strText);
    }
}

VOID InternalDebugOut(LPCWSTR strText)
{
    if(g_DebugOutCallbackW != NULL)
    {
        g_DebugOutCallbackW(strText);
    }
}

VOID DebugOut(LPCSTR pFormat, ...)
{
    CStringA FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

    InternalDebugOut(FormattedString);
}

VOID DebugOut(LPCWSTR pFormat, ...)
{
    CStringW FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

    InternalDebugOut(FormattedString);
}

VOID ZoneOut(LPCSTR pZone, LPCSTR pFormat, ...)
{
    CStringA FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

	CStringA ZoneFormat;

	ZoneFormat.Format("[ %s ] - %s", pZone, (LPCSTR)FormattedString);

    InternalDebugOut(ZoneFormat);
}

VOID ZoneOut(LPCWSTR pZone, LPCWSTR pFormat, ...)
{
    CStringW FormattedString;

    va_list args;
    va_start(args, pFormat);

    FormattedString.FormatV(pFormat, args);

    va_end(args);

	CStringW ZoneFormat;

	ZoneFormat.Format(L"[ %s ] - %s", pZone, (LPCWSTR)FormattedString);

    InternalDebugOut(ZoneFormat);
}

VOID ZoneLevelOut(DWORD Level, LPCSTR pZone, LPCSTR pFormat, ...)
{
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
}

VOID ZoneLevelOut(DWORD Level, LPCWSTR pZone, LPCWSTR pFormat, ...)
{
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
}