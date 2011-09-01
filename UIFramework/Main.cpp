#include "Types.h"

#ifdef _WINDOWS

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