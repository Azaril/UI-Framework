#pragma once

#include "Types.h"

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

typedef void (*DebugOutCallbackFuncA)(const char* strOutput);
typedef void (*DebugOutCallbackFuncW)(const wchar_t* strOutput);

void SetDebugOutCallback(DebugOutCallbackFuncA Func);
void SetDebugOutCallback(DebugOutCallbackFuncW Func);

void InternalDebugOut(const char* strText);
void InternalDebugOut(const wchar_t* strText);

DebugOutCallbackFuncA GetDebugOutCallbackA();
DebugOutCallbackFuncW GetDebugOutCallbackW();

void DebugOut(const char* pFormat, ...);
void DebugOut(const wchar_t* pFormat, ...);
void ZoneOut(const char* pZone, const char* pFormat, ...);
void ZoneOut(const wchar_t* pZone, const wchar_t* pFormat, ...);
void ZoneLevelOut(UINT32 Level, const char* pZone, const char* pFormat, ...);
void ZoneLevelOut(UINT32 Level, const wchar_t* pZone, const wchar_t* pFormat, ...);