#include "UIHost.h"
#include "MouseInput.h"

CUIHost::CUIHost(
    ) 
    : m_RenderTarget(NULL)
    , m_RootElement(NULL)
    , m_MouseController(NULL)
    , m_KeyboardController(NULL)
    , m_FocusManager(NULL)
    , m_TreeData(NULL)
    , m_TimeController(NULL)
    , m_LayoutManager(NULL)
    , m_Time(CTime::Now())
{
}

CUIHost::~CUIHost(
    )
{
    ReleaseObject(m_TimeController);
    ReleaseObject(m_LayoutManager);
    ReleaseObject(m_FocusManager);
    ReleaseObject(m_KeyboardController);
    ReleaseObject(m_MouseController);
    ReleaseObject(m_RootElement);
    ReleaseObject(m_RenderTarget);

    delete m_TreeData;
}

__checkReturn HRESULT 
CUIHost::Initialize(
    __in CGraphicsDevice* pGraphicsDevice,
    __in CRenderTarget* pRenderTarget, 
    __in CProviders* pProviders,
    __in const CFontDescription* pDefaultFont
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(pProviders);

    m_RenderTarget = pRenderTarget;
    AddRefObject(m_RenderTarget);

    m_Time = CTime::Now();

    IFC(CRootUIElement::Create(pProviders, &m_RootElement));

    IFC(CFocusManager::Create(&m_FocusManager));

    IFC(CMouseController::Create(m_RootElement, &m_MouseController));

    IFC(CKeyboardController::Create(m_FocusManager, m_RootElement, &m_KeyboardController));

    IFC(CTimeController::Create(&m_TimeController));

    IFC(CLayoutManager::Create(m_RootElement, &m_LayoutManager));

    m_TreeData = new CStaticTreeData(m_FocusManager, m_MouseController, m_KeyboardController, m_LayoutManager);
    IFCOOM(m_TreeData);

    IFC(m_RootElement->SetContext(pGraphicsDevice, pRenderTarget, m_TimeController, pDefaultFont, m_TreeData));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CUIHost::GetRootElement(
    __deref_out CRootUIElement** ppElement
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppElement);

    *ppElement = m_RootElement;
    AddRefObject(m_RootElement);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CUIHost::GetMouseController(
    __deref_out CMouseController** ppController
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppController);

    *ppController = m_MouseController;
    AddRefObject(m_MouseController);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CUIHost::GetKeyboardController(
    __deref_out CKeyboardController** ppController
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppController);

    *ppController = m_KeyboardController;
    AddRefObject(m_KeyboardController);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CUIHost::GetFocusManager(
    __deref_out CFocusManager** ppFocusManager
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppFocusManager);

    *ppFocusManager = m_FocusManager;
    AddRefObject(m_FocusManager);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CUIHost::GetTimeController(
    __deref_out CTimeController** ppTimeController
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppTimeController);

    *ppTimeController = m_TimeController;
    AddRefObject(m_TimeController);

Cleanup:
    return hr;
}

__checkReturn HRESULT
CUIHost::Render(
    )
{
    HRESULT hr = S_OK;

    IFC(m_LayoutManager->SetLayoutSize(m_RenderTarget->GetSize()));

    {
        CTime CurrentTime = CTime::Now();
        CTimeSpan DeltaTime = CurrentTime - m_Time;
        
        m_Time = CurrentTime;

        IFC(m_TimeController->UpdateTime(DeltaTime));
    }

    IFC(m_LayoutManager->EnsureLayout());

    IFC(m_RootElement->EnsureLoaded());

    IFC(m_LayoutManager->EnsureLayout());

    //{
    //    CPreRenderContext PreRenderContext(m_RenderTarget);

    //    IFC(m_RootElement->PreRender(PreRenderContext));

    //    {
    //        CRenderContext RenderContext(m_RenderTarget);

    //        IFC(PreRenderContext.RenderRoots(RenderContext));
    //    }
    //}

Cleanup:
    return hr;
}

//
// CUIHost
//
extern "C" UIFRAMEWORK_API
void 
CUIHost_AddRef(
    __in CUIHost* pHost
    )
{
    pHost->AddRef();
}

extern "C" UIFRAMEWORK_API
void 
CUIHost_Release(
    __in CUIHost* pHost
    )
{
    pHost->Release();
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT 
CUIHost_GetRootElement(
    __in CUIHost* pHost, 
    __deref_out CRootUIElement** ppElement
    )
{
    return pHost->GetRootElement(ppElement);
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT 
CUIHost_GetFocusManager(
    __in CUIHost* pHost, 
    __deref_out CFocusManager** ppFocusManager
    )
{
    return pHost->GetFocusManager(ppFocusManager);
}