#pragma once

#include "Types.h"
#include "Logging.h"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)


#if _WINDOWS

#include <windows.h>

#else

#define S_OK            (0)
#define S_FALSE         (1)
#define E_FAIL          (-1)
#define E_POINTER       (-2)
#define E_OUTOFMEMORY   (-3)
#define E_UNEXPECTED    (-4)

#define SUCCEEDED(expr) (expr >= 0)
#define FAILED(expr)    (expr < 0)

#endif

#ifdef _DEBUG

#define DEBUG_OUT_HR(expr, res) DebugOut(L"%s(%u): %s\n", __WFILE__, __LINE__, L#expr);

#define IFC(expr) do{ hr = (expr); if(FAILED(hr)) { DEBUG_OUT_HR(expr, hr); goto Cleanup; } } while(FALSE);
#define IFCPTR(ptr) if((ptr) == NULL) { hr = E_POINTER; DEBUG_OUT_HR((ptr) == NULL, hr); goto Cleanup; };
#define IFCOOM(ptr) if((ptr) == NULL) { hr = E_OUTOFMEMORY; DEBUG_OUT_HR((ptr) == NULL, hr); goto Cleanup; };
#define IFCEXPECT(expr) if(!(expr)) { hr = E_UNEXPECTED; DEBUG_OUT_HR(expr, hr); goto Cleanup; };

#else

#define IFC(expr) do{ hr = (expr); if(FAILED(hr)) { goto Cleanup; } } while(FALSE);
#define IFCPTR(ptr) if((ptr) == NULL) { hr = E_POINTER; goto Cleanup; };
#define IFCOOM(ptr) if((ptr) == NULL) { hr = E_OUTOFMEMORY; goto Cleanup; };
#define IFCEXPECT(expr) if(!(expr)) { hr = E_UNEXPECTED; goto Cleanup; };

#endif