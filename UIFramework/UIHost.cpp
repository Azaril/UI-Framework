#include "UIHost.h"
#include "MouseInput.h"

CUIHost::CUIHost() : m_RenderTarget(NULL),
                     m_RootElement(NULL),
                     m_MouseController(NULL)
{
    m_LastLayoutSize.width = 0;
    m_LastLayoutSize.height = 0;
}

CUIHost::~CUIHost()
{
    ReleaseObject(m_MouseController);
    ReleaseObject(m_RootElement);
    ReleaseObject(m_RenderTarget);
}

HRESULT CUIHost::Initialize(CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(pProviders);

    m_RenderTarget = pRenderTarget;
    AddRefObject(m_RenderTarget);

    IFC(CRootUIElement::Create(pGraphicsDevice, pRenderTarget, pProviders, &m_RootElement));

    IFC(CMouseController::Create(m_RootElement, &m_MouseController));

Cleanup:
    return hr;
}

HRESULT CUIHost::GetRootElement(CRootUIElement** ppElement)
{
    HRESULT hr = S_OK;

    IFCPTR(ppElement);

    *ppElement = m_RootElement;
    AddRefObject(m_RootElement);

Cleanup:
    return hr;
}

HRESULT CUIHost::GetMouseController(CMouseController** ppController)
{
    HRESULT hr = S_OK;

    IFCPTR(ppController);

    *ppController = m_MouseController;
    AddRefObject(m_MouseController);

Cleanup:
    return hr;
}

HRESULT CUIHost::EnsureLayout()
{
    HRESULT hr = S_OK;

    SizeF AvailableSize = { 0 };
    SizeF DesiredSize = { 0 };

    AvailableSize = m_RenderTarget->GetSize();

    if(m_LastLayoutSize.width != AvailableSize.width || m_LastLayoutSize.height != AvailableSize.height)
    {
        m_RootElement->InvalidateMeasure();

        m_LastLayoutSize = AvailableSize;
    }

    if(m_RootElement->IsMeasureDirty())
    {
        IFC(m_RootElement->Measure(AvailableSize));
    }

    if(m_RootElement->IsArrangeDirty())
    {
        IFC(m_RootElement->Arrange(MakeRect(AvailableSize)));
    }

Cleanup:
    return hr;
}

HRESULT CUIHost::Render()
{
    HRESULT hr = S_OK;

    IFC(EnsureLayout());

    {
        CPreRenderContext PreRenderContext(m_RenderTarget);

        IFC(m_RootElement->PreRender(PreRenderContext));

        {
            CRenderContext RenderContext(m_RenderTarget);

            IFC(PreRenderContext.RenderRoots(RenderContext));
        }
    }

Cleanup:
    return hr;
}