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
#include "StaticClassResolver.h"
#include "BasicTypeConverter.h"

#include <d2d1helper.h>

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

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    HRESULT hr = S_OK;
    HWND hWnd = NULL;
    MSG msg = { 0 };
	HACCEL hAccelTable = NULL;
    CGraphicsDevice* pGraphicsDevice = NULL;
    CImagingProvider* pImagingProvider = NULL;
    CRenderTarget* pRenderTarget = NULL;
    CUIHost* pUIHost = NULL;
    CRootUIElement* pRootElement = NULL;
    CCanvas* pCanvas = NULL;
    CStackPanel* pStackPanel = NULL;
    CStackPanel* pChildStackPanel = NULL;
    CBrush* pWhiteSolidBrush = NULL;
    CBrush* pBlueSolidBrush = NULL;
    CBrush* pRedSolidBrush = NULL;
    CBitmapSource* pBitmapSource = NULL;
    CBorder* pRootBorder = NULL;    
    CUIElement* pParsedRoot = NULL;

    CParser* pParser = NULL;
    CStaticClassResolver* pStaticClassResolver = NULL;
    CTypeConverter* pTypeConverter = NULL;

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
    // Create Parser
    // 
    IFC(CStaticClassResolver::Create(&pStaticClassResolver));

    IFC(CreateBasicTypeConverter(&pTypeConverter));

    IFC(CParser::Create(pStaticClassResolver, pTypeConverter, &pParser));

    IFC(pParser->LoadFromFile(L"c:\\testxml.xml", &pParsedRoot));

    //
    // Create UI framework
    //
    IFC(CreateGraphicsDevice(&pGraphicsDevice));

    IFC(pGraphicsDevice->CreateHWNDRenderTarget(hWnd, &pRenderTarget));

    IFC(pRenderTarget->CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteSolidBrush));
    IFC(pRenderTarget->CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &pBlueSolidBrush));
    IFC(pRenderTarget->CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedSolidBrush));

    g_RenderTarget = (CD2DHWNDRenderTarget*)pRenderTarget;

    IFC(CUIHost::Create(pGraphicsDevice, pRenderTarget, &pUIHost));

    IFC(pUIHost->GetRootElement(&pRootElement));

    IFC(pRootElement->SetChild(pParsedRoot));

    //IFC(CBorder::Create(&pRootBorder));
    //IFC(pRootBorder->SetBackgroundBrush(pWhiteSolidBrush));
    //IFC(pRootElement->SetChild(pRootBorder));

    //IFC(CCanvas::Create(&pCanvas));

    //IFC(CStackPanel::Create(&pStackPanel));
    //IFC(pStackPanel->SetOrientation(Orientation::Vertical));    

    //IFC(pRootBorder->SetChild(pStackPanel));

    //IFC(pGraphicsDevice->GetImagingProvider(&pImagingProvider));

    //IFC(pImagingProvider->LoadBitmapFromFile(L"C:\\3.png", &pBitmapSource));

    ////IFC(pRootElement->SetBackground(pBlueSolidBrush));

    //for(unsigned int j = 0; j < 10; j++)
    //{
    //    IFC(CStackPanel::Create(&pChildStackPanel));

    //    IFC(pChildStackPanel->SetOrientation(Orientation::Horizontal));

    //    IFC(pStackPanel->AddChild(pChildStackPanel));

    //    for(unsigned int i = 0; i < 10; i++)
    //    {
    //        CTextBlock* pTextBlock = NULL;
    //        CBorder* pBorder = NULL;
    //        CImage* pImage = NULL;

    //        SizeF Size = { 20, 20 };
    //        Point2F Position = { 20.0f * i, 20.0f * i };
    //        RectF Border = { 4, 4, 4, 4 };

    //        IFC(CBorder::Create(&pBorder));
    //        IFC(CTextBlock::Create(&pTextBlock));
    //        IFC(CImage::Create(&pImage));

    //        //IFC(pBorder->SetSize(Size));
    //        //IFC(pBorder->SetBackgroundBrush(pBlueSolidBrush));
    //        
    //        IFC(pBorder->SetBorderThickness(Border));
    //        IFC(pBorder->SetBorderBrush(pRedSolidBrush));
    //        IFC(pBorder->SetPadding(Border));

    //        //IFC(pTextBlock->SetSize(Size));
    //        //IFC(pTextBlock->SetBackground(pRedSolidBrush));

    //        //IFC(pBorder->SetChild(pTextBlock));
    //        IFC(pBorder->SetChild(pImage));

    //        //IFC(pCanvas->SetChildPosition(pTextBlock, Position));

    //        IFC(pTextBlock->SetText(L"Testing!"));

    //        IFC(pImage->SetSource(pBitmapSource));

    //        IFC(pChildStackPanel->AddChild(pBorder));
    //        //IFC(pChildStackPanel->AddChild(pImage));
    //        IFC(pChildStackPanel->AddChild(pTextBlock));

    //        ReleaseObject(pTextBlock);
    //        ReleaseObject(pBorder);
    //    }

    //    ReleaseObject(pChildStackPanel);
    //}

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
            IFC(pUIHost->Render());
        }
    } while(msg.message != WM_QUIT);


Cleanup:
    ReleaseObject(pParsedRoot);
    ReleaseObject(pWhiteSolidBrush);
    ReleaseObject(pBlueSolidBrush);
    ReleaseObject(pRedSolidBrush);
    ReleaseObject(pRootElement);
    ReleaseObject(pRootBorder);
    ReleaseObject(pUIHost);
    ReleaseObject(pRenderTarget);
    ReleaseObject(pImagingProvider);
    ReleaseObject(pGraphicsDevice);
    ReleaseObject(pBitmapSource);
    ReleaseObject(pStaticClassResolver);
    ReleaseObject(pTypeConverter);
    ReleaseObject(pParser);

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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
    case WM_SIZE:
        {
            if(wParam == SIZE_RESTORED)
            {
                ID2D1HwndRenderTarget* pRenderTarget = g_RenderTarget->GetD2DHWNDRenderTarget();
                
                pRenderTarget->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
            }
            break;
        }

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		//switch (wmId)
		//{
		//default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		//}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}