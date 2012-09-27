#pragma once

#if defined(WIN32) || defined(_XBOX)

#if defined (_WINDLL)

#ifdef UIFRAMEWORK_EXPORTS
#define UIFRAMEWORK_API __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define UIFRAMEWORK_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif

#else

#define UIFRAMEWORK_API
#define EXPIMP_TEMPLATE extern

#endif

#include "sal.h"

#pragma warning(disable: 4251)

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

#if !defined(FRAMEWORK_DEBUG) || !defined(FRAMEWORK_RELEASE)

#if defined(DEBUG) || defined(_DEBUG)
#if !defined(FRAMEWORK_DEBUG)
#define FRAMEWORK_DEBUG
#endif
#else
#if !defined(FRAMEWORK_RELEASE)
#define FRAMEWORK_RELEASE
#endif
#endif

#endif

#if defined(FRAMEWORK_DEBUG)
#undef FRAMEWORK_RELASE
#endif