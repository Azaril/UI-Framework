#pragma once

#include "Types.h"

#if _WINDOWS

#include <windows.h>

#else

#define S_OK            (0)
#define E_FAIL          (-1)
#define E_POINTER       (-2)
#define E_OUTOFMEMORY   (-3)

#define SUCCEEDED(expr) (expr >= 0)
#define FAILED(expr)    (expr < 0)

#endif

#define IFC(expr) do{ hr = (expr); if(FAILED(hr)) { goto Cleanup; } } while(FALSE);
#define IFCPTR(ptr) if((ptr) == NULL) { hr = E_POINTER; goto Cleanup; };
#define IFCOOM(ptr) if((ptr) == NULL) { hr = E_OUTOFMEMORY; goto Cleanup; };
#define IFCEXPECT(expr) if(!(expr)) { hr = E_UNEXPECTED; goto Cleanup; };