#pragma once

#include "Types.h"

namespace logging
{
//#include "PublicUtilities.h"

#define LOGGING_TRACE       4
#define LOGGING_VERBOSE     3
#define LOGGING_INFO        2
#define LOGGING_CRITICAL    1
#define LOGGING_OFF         0

#ifdef DEBUG
#define LOGGING_LEVEL   LOGGING_VERBOSE
#else
#define LOGGING_LEVEL   LOGGING_VERBOSE
#endif

#if LOGGING_LEVEL > LOGGING_OFF
#define DEBUG_OUT	    DebugOut
#define ZONE_OUT	    ZoneOut
#define ZONE_LEVEL_OUT  ZoneLevelOut
#else
#define DEBUG_OUT(a)
#define ZONE_OUT(a)
#define ZONE_LEVEL_OUT(a)
#endif

//#include "Common.h"
//#include "LoggingMacros.h"

typedef void (*DebugOutCallbackFuncA)(
    __in_z const char* strOutput
    );

typedef void (*DebugOutCallbackFuncW)(
    __in_z const wchar_t* strOutput
    );

void UIFRAMEWORK_API SetDebugOutCallback(
    __in_opt DebugOutCallbackFuncA Func
    );

void UIFRAMEWORK_API SetDebugOutCallback(
    __in_opt DebugOutCallbackFuncW Func
    );

void InternalDebugOut(
    __in_z const char* strText
    );

void InternalDebugOut(
    __in_z const wchar_t* strText
    );

__out_opt DebugOutCallbackFuncA UIFRAMEWORK_API GetDebugOutCallbackA(
    );

__out_opt DebugOutCallbackFuncW UIFRAMEWORK_API GetDebugOutCallbackW(
    );

void UIFRAMEWORK_API DebugOut(
    __in_z const char* pFormat, 
    ...
    );

void UIFRAMEWORK_API DebugOut( 
    __in_z __format_string const wchar_t* pFormat, 
    ...
    );

void UIFRAMEWORK_API ZoneOut(
    __in_z const char* pZone,
    __in_z __format_string const char* pFormat, 
    ...
    );

void UIFRAMEWORK_API ZoneOut(
    __in_z const wchar_t* pZone, 
    __in_z __format_string const wchar_t* pFormat, 
    ...
    );

void UIFRAMEWORK_API ZoneLevelOut(
    UINT32 Level, 
    __in_z const char* pZone, 
    __in_z __format_string const char* pFormat, 
    ...
    );

void UIFRAMEWORK_API ZoneLevelOut(
    UINT32 Level, 
    __in_z const wchar_t* pZone, 
    __in_z __format_string const wchar_t* pFormat, 
    ...
    );
}