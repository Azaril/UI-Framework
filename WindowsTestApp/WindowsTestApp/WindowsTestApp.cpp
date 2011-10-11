#include "stdafx.h"
#include "WindowsTestApp.h"

#include "ErrorChecking.h"
#include "GraphicsDevice.h"
#include "UIHost.h"
#include "Canvas.h"
#include "TextBlock.h"
#include "StackPanel.h"
#include "Border.h"
#include "Image.h"
#include "Parser.h"
#include "DynamicClassResolver.h"
#include "BasicTypeConverter.h"
#include "FileResourceProvider.h"

#include <Windowsx.h>
#include <strsafe.h>

//#define BUILD_D2D
#define BUILD_D3D9

#if defined(BUILD_D2D)

#include <d2d1helper.h>
#include "D2DHWNDRenderTarget.h"
#include "D2DGraphicsDevice.h"

#elif defined(BUILD_D3D9)

#include "D3D9GraphicsDevice.h"
#include "D3D9RenderTarget.h"

#elif defined(BUILD_IRRLICHT)

#include "irrlicht.h"
#include "IrrGraphicsDevice.h"
#include "IrrTextureRenderTarget.h"

#endif

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass( HINSTANCE hInstance );
HRESULT InitInstance( HINSTANCE hInstance, int nCmdShow, HWND* pWindow );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

#if defined(BUILD_D2D)

CD2DHWNDRenderTarget* g_RenderTarget = NULL;

#elif defined(BUILD_D3D9)

CD3D9RenderTarget* g_RenderTarget = NULL;

#elif defined(BUILD_IRRLICHT)


#endif

CUIHost* g_UIHost = NULL;
CMouseController* g_MouseController = NULL;
CKeyboardController* g_KeyboardController = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    HRESULT hr = S_OK;
    CUIHost* pUIHost = NULL;
    CRootUIElement* pRootElement = NULL; 
    CUIElement* pParsedRoot = NULL;
    CParser* pParser = NULL;
    CDynamicClassResolver* pClassResolver = NULL;
    CTypeConverter* pTypeConverter = NULL;
    CProviders* pProviders = NULL;
    CMouseController* pMouseController = NULL;
    CKeyboardController* pKeyboardController = NULL;
    CFileResourceProvider* pFileResourceProvider = NULL;

#if defined(BUILD_D2D) || defined(BUILD_D3D9)

    HWND hWnd = NULL;
    MSG msg = { 0 };
    HACCEL hAccelTable = NULL;

#endif 

#if defined(BUILD_D2D)

    CD2DGraphicsDevice* pGraphicsDevice = NULL;
    CD2DHWNDRenderTarget* pRenderTarget = NULL;

#elif defined(BUILD_D3D9)

    CD3D9GraphicsDevice* pGraphicsDevice = NULL;
    CD3D9HWNDRenderTarget* pRenderTarget = NULL;

#elif defined(BUILD_IRRLICHT)

    irr::IrrlichtDevice* pIrrlichtDevice = NULL;
    CIrrlichtGraphicsDevice* pGraphicsDevice = NULL;
    CIrrlichtTextureRenderTarget* pIrrlichtRenderTarget = NULL;

#endif

#if defined(_DEBUG) && defined(USE_LEAK_CHECKING)
    // Get current flag
    int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

    // Turn on leak-checking bit.
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;

    //// Turn off CRT block checking bit.
    //tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;

    // Set flag to the new value.
    _CrtSetDbgFlag( tmpFlag );
#endif

    //
    // Create providers
    //
    IFC(CDynamicClassResolver::Create(&pClassResolver));

    IFC(CreateBasicTypeConverter(&pTypeConverter));

    IFC(CFileResourceProvider::Create(&pFileResourceProvider));

    IFC(CProviders::Create(pClassResolver, pTypeConverter, pFileResourceProvider, &pProviders));

    //
    // Create Parser
    // 
    IFC(CParser::Create(pProviders, &pParser));

    IFC(pParser->LoadFromFile(L"c:\\testxml.xml", &pParsedRoot));

#if defined(BUILD_D2D) || defined(BUILD_D3D9)

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINDOWSTESTAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

    //
    // Create window
    //
	IFC(InitInstance (hInstance, nCmdShow, &hWnd));

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSTESTAPP));
    IFCEXPECT(hAccelTable != NULL);

#endif

    //
    // Create UI framework
    //
#if defined(BUILD_D2D)

    IFC(CD2DGraphicsDevice::Create(&pGraphicsDevice));

    IFC(pGraphicsDevice->CreateHWNDRenderTarget(hWnd, &pRenderTarget));

#elif defined(BUILD_D3D9)

    IFC(CD3D9GraphicsDevice::Create(&pGraphicsDevice));

    IFC(pGraphicsDevice->CreateHWNDRenderTarget(hWnd, &pRenderTarget));

#elif defined(BUILD_IRRLICHT)

    {
        HMODULE IrrlichtModule = LoadLibrary(L"Irrlicht.dll");
        IFCEXPECT(IrrlichtModule != NULL);

        irr::funcptr_createDevice pCreateDevice = (irr::funcptr_createDevice)GetProcAddress(IrrlichtModule, "createDevice");
        IFCPTR(pCreateDevice);

        pIrrlichtDevice = pCreateDevice(irr::video::EDT_DIRECT3D9, irr::core::dimension2d<irr::u32>(640, 480), 16, false, false, false, NULL);
        IFCPTR(pIrrlichtDevice);

        IFC(CIrrlichtGraphicsDevice::Create(pIrrlichtDevice->getVideoDriver(), &pGraphicsDevice));

        IFC(pGraphicsDevice->CreateRenderTarget(SizeF(640, 480), &pRenderTarget));

        pIrrlichtRenderTarget = (CIrrlichtTextureRenderTarget*)pRenderTarget;
    }

#endif

    g_RenderTarget = pRenderTarget;

    IFC(CUIHost::Create(pGraphicsDevice, pRenderTarget, pProviders, &pUIHost));

    g_UIHost = pUIHost;

    IFC(pUIHost->GetMouseController(&pMouseController));

    g_MouseController = pMouseController;

    IFC(pUIHost->GetKeyboardController(&pKeyboardController));

    g_KeyboardController = pKeyboardController;

    IFC(pUIHost->GetRootElement(&pRootElement));

    IFC(pRootElement->SetChild(pParsedRoot));

    //
    // Message pump
    //
#if defined(BUILD_D2D) || defined(BUILD_D3D9)

    do
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
	        TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            IFC(pUIHost->Render());
        }
    } while(msg.message != WM_QUIT);

#elif defined(BUILD_IRRLICHT)

    while(pIrrlichtDevice->run())
    {
        IFC(pUIHost->Render());

        irr::video::IVideoDriver* pDriver = pIrrlichtDevice->getVideoDriver();

        IFCEXPECT(pDriver->setRenderTarget(irr::video::ERT_FRAME_BUFFER));

        IFCEXPECT(pDriver->beginScene());

        pDriver->draw2DImage(pIrrlichtRenderTarget->GetIrrlichtTexture(), irr::core::vector2d<irr::s32>(0, 0));

        IFCEXPECT(pDriver->endScene());
    }

#endif

Cleanup:
    ReleaseObject(pMouseController);
    ReleaseObject(pKeyboardController);
    ReleaseObject(pParsedRoot);
    ReleaseObject(pRootElement);
    ReleaseObject(pUIHost);
    ReleaseObject(pRenderTarget);
    ReleaseObject(pClassResolver);
    ReleaseObject(pTypeConverter);
    ReleaseObject(pParser);
    ReleaseObject(pFileResourceProvider);
    ReleaseObject(pProviders);

    ReleaseObject(pGraphicsDevice);

#if defined(BUILD_D2D)

#elif defined(BUILD_IRRLICHT)

    ReleaseObject(pIrrlichtDevice);

#endif

	return SUCCEEDED(hr);
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSTESTAPP));
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

HRESULT UpdateRenderTargetSize(HWND Window)
{
    HRESULT hr = S_OK;
    RECT ClientSize = { 0 };
    
    IFCEXPECT(GetClientRect(Window, &ClientSize));

    if(g_RenderTarget != NULL)
    {
#if defined(BUILD_D2D)

        ID2D1HwndRenderTarget* pRenderTarget = g_RenderTarget->GetD2DHWNDRenderTarget();

        IFC(pRenderTarget->Resize(D2D1::SizeU(ClientSize.right - ClientSize.left, ClientSize.bottom - ClientSize.top)));

#elif defined(BUILD_D3D9)

        //TODO: Handle resize for D3D9.

#endif                   
    }

Cleanup:
    return hr;
}

BOOL g_Sizing = FALSE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = S_OK;
    LRESULT Result = 0;
    BOOL Handled = FALSE;

	switch (message)
	{
        case WM_SIZE:
            {
                if(!g_Sizing)
                {
                    if(wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED)
                    {
                        IFC(UpdateRenderTargetSize(hWnd));
                    }
                }

                break;
            }

        case WM_ENTERSIZEMOVE:
            {
                g_Sizing = TRUE;

                break;
            }

        case WM_EXITSIZEMOVE:
            {
                g_Sizing = FALSE;

                IFC(UpdateRenderTargetSize(hWnd));

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

                IFC(g_MouseController->InjectMouseMove(Point));

                IFC(g_MouseController->InjectMouseButton(MouseButton::Left, MouseButtonState::Down));

                Handled = TRUE;

                break;
            }

        case WM_LBUTTONUP:
            {
                Point2F Point((FLOAT)GET_X_LPARAM(lParam), (FLOAT)GET_Y_LPARAM(lParam));

                IFC(g_MouseController->InjectMouseMove(Point));

                IFC(g_MouseController->InjectMouseButton(MouseButton::Left, MouseButtonState::Up));

                Handled = TRUE;

                break;
            }

        case WM_KEYDOWN:
            {
                BOOL Consumed = FALSE;

                //TODO: Convert to key value.
                //IFC(g_KeyboardController->InjectKey(wParam, KeyState::Down, &Consumed));

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

                //TODO: Convert to key value.
                //IFC(g_KeyboardController->InjectKey(wParam, KeyState::Up, &Consumed));

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