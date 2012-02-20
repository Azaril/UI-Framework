#pragma once

#include "resource.h"

ATOM RegisterWindowClass( 
    __in HINSTANCE hInstance 
    );

__checkReturn HRESULT InitInstance( 
    __in HINSTANCE hInstance, 
    int nCmdShow, 
    __out HWND* pWindow 
    );

LRESULT CALLBACK WndProc( 
    HWND hWnd, 
    UINT message, 
    WPARAM wParam,
    LPARAM lParam 
    );

void CheckLeaks(
    );