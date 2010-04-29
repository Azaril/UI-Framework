#pragma once

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

typedef VOID (*DebugOutCallbackFuncA)(LPCSTR strOutput);
typedef VOID (*DebugOutCallbackFuncW)(LPCWSTR strOutput);

VOID SetDebugOutCallback(DebugOutCallbackFuncA Func);
VOID SetDebugOutCallback(DebugOutCallbackFuncW Func);

VOID InternalDebugOut(LPCSTR strText);
VOID InternalDebugOut(LPCWSTR strText);

DebugOutCallbackFuncA GetDebugOutCallbackA();
DebugOutCallbackFuncW GetDebugOutCallbackW();

VOID DebugOut(LPCSTR pFormat, ...);
VOID DebugOut(LPCWSTR pFormat, ...);
VOID ZoneOut(LPCSTR pZone, LPCSTR pFormat, ...);
VOID ZoneOut(LPCWSTR pZone, LPCWSTR pFormat, ...);
VOID ZoneLevelOut(DWORD Level, LPCSTR pZone, LPCSTR pFormat, ...);
VOID ZoneLevelOut(DWORD Level, LPCWSTR pZone, LPCWSTR pFormat, ...);