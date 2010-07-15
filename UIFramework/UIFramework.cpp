// UIFramework.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "UIFramework.h"

#include "ErrorChecking.h"
#include "GraphicsDevice.h"
#include "UIHost.h"
#include "Canvas.h"
#include "TextBlock.h"
#include "StackPanel.h"
#include "Border.h"
#include "Image.h"
#include "D2DHWNDRenderTarget.h"
#include "Parser.h"
#include "DynamicClassResolver.h"
#include "BasicTypeConverter.h"

#include <d2d1helper.h>
#include <Windowsx.h>
#include <strsafe.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass( HINSTANCE hInstance );
HRESULT InitInstance( HINSTANCE hInstance, int nCmdShow, HWND* pWindow );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

CD2DHWNDRenderTarget* g_RenderTarget = NULL;
CUIHost* g_UIHost = NULL;
CMouseController* g_MouseController = NULL;
CKeyboardController* g_KeyboardController = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    HRESULT hr = S_OK;
    HWND hWnd = NULL;
    MSG msg = { 0 };
	HACCEL hAccelTable = NULL;
    CGraphicsDevice* pGraphicsDevice = NULL;
    CRenderTarget* pRenderTarget = NULL;
    CUIHost* pUIHost = NULL;
    CRootUIElement* pRootElement = NULL; 
    CUIElement* pParsedRoot = NULL;
    CParser* pParser = NULL;
    CDynamicClassResolver* pClassResolver = NULL;
    CTypeConverter* pTypeConverter = NULL;
    CTextBlock* pTextBlock1 = NULL;
    CImage* pImage1 = NULL;
    CBrush* pWhiteBrush = NULL;
    CProviders* pProviders = NULL;
    CMouseController* pMouseController = NULL;
    CKeyboardController* pKeyboardController = NULL;

#ifdef _DEBUG
    // Get current flag
    int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

    // Turn on leak-checking bit.
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;

    //// Turn off CRT block checking bit.
    //tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;

    // Set flag to the new value.
    _CrtSetDbgFlag( tmpFlag );
#endif

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_UIFRAMEWORK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

    //
    // Create window
    //
	IFC(InitInstance (hInstance, nCmdShow, &hWnd));

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UIFRAMEWORK));
    IFCEXPECT(hAccelTable != NULL);

    //
    // Create providers
    //
    IFC(CDynamicClassResolver::Create(&pClassResolver));

    IFC(CreateBasicTypeConverter(&pTypeConverter));

    IFC(CProviders::Create(pClassResolver, pTypeConverter, &pProviders));

    //
    // Create Parser
    // 
    IFC(CParser::Create(pProviders, &pParser));

    IFC(pParser->LoadFromFile(L"c:\\testxml.xml", &pParsedRoot));

    //
    // Create UI framework
    //
    IFC(CreateGraphicsDevice(&pGraphicsDevice));

    IFC(pGraphicsDevice->CreateHWNDRenderTarget(hWnd, &pRenderTarget));

    g_RenderTarget = (CD2DHWNDRenderTarget*)pRenderTarget;

    IFC(CUIHost::Create(pGraphicsDevice, pRenderTarget, pProviders, &pUIHost));

    g_UIHost = pUIHost;

    IFC(pUIHost->GetMouseController(&pMouseController));

    g_MouseController = pMouseController;

    IFC(pUIHost->GetKeyboardController(&pKeyboardController));

    g_KeyboardController = pKeyboardController;

    IFC(pUIHost->GetRootElement(&pRootElement));

    IFC(pRootElement->SetChild(pParsedRoot));

    //IFC(pRootElement->FindName(L"TextBlock1", &pTextBlock1));

    //IFC(pRootElement->FindName(L"Image1", &pImage1));

    ////IFC(pImage1->FindResource(L"WhiteBrush", &pWhiteBrush));

    //
    // Message pump
    //
    do
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
	        TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //WCHAR TextBuffer[1024];

            //IFC(StringCchPrintf(TextBuffer, ARRAYSIZE(TextBuffer), L"%u", GetTickCount()));

            //IFC(pTextBlock1->SetText(TextBuffer));

            IFC(pUIHost->Render());
        }
    } while(msg.message != WM_QUIT);


Cleanup:
    ReleaseObject(pMouseController);
    ReleaseObject(pKeyboardController);
    ReleaseObject(pWhiteBrush);
    ReleaseObject(pTextBlock1);
    ReleaseObject(pImage1);
    ReleaseObject(pParsedRoot);
    ReleaseObject(pRootElement);
    ReleaseObject(pUIHost);
    ReleaseObject(pRenderTarget);
    ReleaseObject(pGraphicsDevice);
    ReleaseObject(pClassResolver);
    ReleaseObject(pTypeConverter);
    ReleaseObject(pParser);
    ReleaseObject(pProviders);

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UIFRAMEWORK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HRESULT InitInstance(HINSTANCE hInstance, int nCmdShow, HWND* pWindow)
{
    HRESULT hr = S_OK;
    HWND hWnd = NULL;

    IFCPTR(pWindow);

    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 640, 480, NULL, NULL, hInstance, NULL);
    IFCEXPECT(hWnd != NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    *pWindow = hWnd;

Cleanup:
   return hr;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = S_OK;
    LRESULT Result = 0;
    BOOL Handled = FALSE;

	switch (message)
	{
        case WM_SIZE:
            {
                if(wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED)
                {
                    ID2D1HwndRenderTarget* pRenderTarget = g_RenderTarget->GetD2DHWNDRenderTarget();
                    
                    IFC(pRenderTarget->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam))));
                }
                break;
            }

        case WM_MOUSEMOVE:
            {
                Point2F Point((FLOAT)GET_X_LPARAM(lParam), (FLOAT)GET_Y_LPARAM(lParam));

                IFC(g_MouseController->InjectMouseMove(Point));

                Handled = TRUE;

                break;
            }

        case WM_LBUTTONDOWN:
            {
                Point2F Point((FLOAT)GET_X_LPARAM(lParam), (FLOAT)GET_Y_LPARAM(lParam));

                IFC(g_MouseController->InjectMouseButton(MouseButton::Left, MouseButtonState::Down, Point));

                Handled = TRUE;

                break;
            }

        case WM_LBUTTONUP:
            {
                Point2F Point((FLOAT)GET_X_LPARAM(lParam), (FLOAT)GET_Y_LPARAM(lParam));

                IFC(g_MouseController->InjectMouseButton(MouseButton::Left, MouseButtonState::Up, Point));

                Handled = TRUE;

                break;
            }

        case WM_KEYDOWN:
            {
                BOOL Consumed = FALSE;

                IFC(g_KeyboardController->InjectKey(wParam, KeyState::Down, &Consumed));

                Handled = Consumed;

                if(Consumed)
                {
                    Result = 0;
                }

                break;
            }

        case WM_KEYUP:
            {
                BOOL Consumed = FALSE;

                IFC(g_KeyboardController->InjectKey(wParam, KeyState::Up, &Consumed));

                Handled = Consumed;

                if(Consumed)
                {
                    Result = 0;
                }

                break;
            }

        case WM_CHAR:
            {
                if(iswprint(wParam))
                {
                    BOOL Consumed = FALSE;

                    IFC(g_KeyboardController->InjectCharacter(wParam, &Consumed));

                    Handled = Consumed;
                    
                    if(Consumed)
                    {
                        Result = 0;
                    }
                }

                break;
            }

	    case WM_DESTROY:
            {
		        PostQuitMessage(0);

                Handled = TRUE;

		        break;
            }
	}

Cleanup:
    if(!Handled)
    {
        Result = DefWindowProc(hWnd, message, wParam, lParam);
    }

	return Result;
}