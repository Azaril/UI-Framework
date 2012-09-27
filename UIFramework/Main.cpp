#include "Types.h"

#if defined(_WINDLL)

#include <Windows.h>

BOOL APIENTRY 
DllMain(
	HMODULE Module, 
	DWORD ReasonForCall, 
	LPVOID Reserved
	)
{
    return TRUE;
}

#endif