#pragma once

#ifdef WIN32

#ifdef UIFRAMEWORK_EXPORTS
#define UIFRAMEWORK_API __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define UIFRAMEWORK_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif

#else

#ifdef UIFRAMEWORK_EXPORTS
#define UIFRAMEWORK_API __attribute__((visibility("default")))
#define EXPIMP_TEMPLATE
#else
#define UIFRAMEWORK_API 
#define EXPIMP_TEMPLATE extern
#endif

#include "DummySal.h"

#endif