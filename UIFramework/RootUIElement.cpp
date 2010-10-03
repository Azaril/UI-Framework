#include "RootUIElement.h"

CRootUIElement::CRootUIElement() : m_Child(NULL),
                                   m_Namescope(NULL),
                                   m_Providers(NULL)
{
}

CRootUIElement::~CRootUIElement()
{
    Finalize();
}

HRESULT CRootUIElement::Initialize(CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CProviders* pProviders, CStaticTreeData* pTreeData)
{
    HRESULT hr = S_OK;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(pProviders);
    IFCPTR(pTreeData);

    IFC(CFrameworkElement::Initialize(pProviders));

    IFC(CNamescope::Create(&m_Namescope));

    m_Providers = pProviders;
    AddRefObject(m_Providers);

    {
        CVisualAttachContext VisualContext(NULL, pGraphicsDevice);

        IFC(OnVisualAttach(VisualContext));
    }

    {
        CUIAttachContext UIContext(pTreeData, NULL, NULL, m_Namescope);

        IFC(OnAttach(UIContext));
    }

Cleanup:
    return hr;
}

HRESULT CRootUIElement::Finalize()
{
    HRESULT hr = S_OK;

    {
        CUIDetachContext UIContext;

        IFC(OnDetach(UIContext));
    }

    {
        CVisualDetachContext VisualContext(this, m_VisualContext.GetGraphicsDevice());

        IFC(OnVisualDetach(VisualContext));
    }

Cleanup:
    ReleaseObject(m_Child);
    ReleaseObject(m_Namescope);
    ReleaseObject(m_Providers);

    return hr;
}

HRESULT CRootUIElement::SetChild(CUIElement* pChild)
{
    HRESULT hr = S_OK;

    if(m_Child)
    {
        IFC(RemoveLogicalChild(m_Child));

        ReleaseObject(m_Child);
    }

    m_Child = pChild;

    if(m_Child)
    {
        AddRefObject(m_Child);

        IFC(AddLogicalChild(m_Child));
    }

Cleanup:
    return hr;
}

HRESULT CRootUIElement::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    IFC(Context.AddRenderRoot(this));

    IFC(CFrameworkElement::PreRenderInternal(Context));

Cleanup:
    return hr;
}

HRESULT CRootUIElement::RenderRoot(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    ColorF ClearColor;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    IFC(pRenderTarget->BeginRendering());

    IFC(pRenderTarget->Clear(ClearColor));

    IFC(Render(Context));

    IFC(pRenderTarget->EndRendering());

Cleanup:
    return hr;
}

HRESULT CRootUIElement::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;

    if(m_Child)
    {
        IFC(m_Child->Measure(AvailableSize));
    }

    DesiredSize = AvailableSize;

Cleanup:
    return hr;
}

HRESULT CRootUIElement::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;

    if(m_Child)
    {
        IFC(m_Child->Arrange(MakeRect(AvailableSize)));
    }

    UsedSize = AvailableSize;

Cleanup:
    return hr;
}

HRESULT CRootUIElement::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    SizeF FinalSize = GetFinalSize();

    if(LocalPoint.x >= 0 && LocalPoint.y >= 0 && LocalPoint.x < FinalSize.width && LocalPoint.y < FinalSize.height)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

Cleanup:
    return hr;
}

//
// CRootUIElement
//
extern "C" __declspec(dllexport)
TypeIndex::Value CRootUIElement_TypeIndex()
{
    return TypeIndex::RootUIElement;
}

extern "C" __declspec(dllexport)
CFrameworkElement* CRootUIElement_CastTo_CFrameworkElement(CRootUIElement* pElement)
{
    return pElement;
}

extern "C" __declspec(dllexport)
CRootUIElement* CObjectWithType_CastTo_CRootUIElement(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::RootUIElement)) ? (CRootUIElement*)pObject : NULL;
}