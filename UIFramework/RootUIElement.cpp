#include "RootUIElement.h"

CRootUIElement::CRootUIElement() : m_Child(NULL)
{
}

CRootUIElement::~CRootUIElement()
{
    Finalize();
}

HRESULT CRootUIElement::Initialize(CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget)
{
    HRESULT hr = S_OK;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);

    IFC(CFrameworkElement::Initialize());

    {
        CVisualAttachContext VisualContext(pGraphicsDevice);

        IFC(OnVisualAttach(VisualContext));
    }

    {
        CUIAttachContext UIContext;

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
        CVisualDetachContext VisualContext(m_VisualContext.GetGraphicsDevice());

        IFC(OnVisualDetach(VisualContext));
    }

    ReleaseObject(m_Child);

Cleanup:
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

HRESULT CRootUIElement::PreRender(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    IFC(Context.AddRenderRoot(this));

    IFC(CFrameworkElement::PreRender(Context));

Cleanup:
    return hr;
}

HRESULT CRootUIElement::RenderRoot(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    ColorF ClearColor = { 1, 1, 1, 1 };

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

HRESULT CRootUIElement::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Arrange(Size));

    if(m_Child)
    {
        IFC(m_Child->Arrange(Size));
    }

Cleanup:
    return hr;
}