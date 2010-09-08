#pragma once

#ifdef UIFRAMEWORK_EXPORTS
#define UIFRAMEWORK_API __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define UIFRAMEWORK_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif