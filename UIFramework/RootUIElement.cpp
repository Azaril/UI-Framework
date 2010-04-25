#include "RootUIElement.h"

CRootUIElement::CRootUIElement() : m_Content(NULL)
{
}

CRootUIElement::~CRootUIElement()
{
    Finalize();

    ReleaseObject(m_Content);
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
        CVisualDetachContext VisualContext;

        IFC(OnVisualDetach(VisualContext));
    }

Cleanup:
    return hr;
}

HRESULT CRootUIElement::SetContent(CUIElement* pContent)
{
    HRESULT hr = S_OK;

    if(m_Content)
    {
        IFC(RemoveLogicalChild(m_Content));

        ReleaseObject(m_Content);
    }

    m_Content = pContent;

    if(m_Content)
    {
        AddRefObject(m_Content);

        IFC(AddLogicalChild(m_Content));
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
    ColorF ClearColor = { 0, 0, 0, 0 };

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

    if(m_Content)
    {
        IFC(m_Content->Measure(AvailableSize));
    }

    DesiredSize = AvailableSize;

Cleanup:
    return hr;
}

HRESULT CRootUIElement::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Arrange(Size));

    if(m_Content)
    {
        IFC(m_Content->Arrange(Size));
    }

Cleanup:
    return hr;
}