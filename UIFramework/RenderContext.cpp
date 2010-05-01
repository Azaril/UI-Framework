#include "RenderContext.h"
#include "ErrorChecking.h"

CRenderContext::CRenderContext(CRenderTarget* pRenderTarget) : m_RenderTarget(pRenderTarget)
{
}

CRenderContext::~CRenderContext()
{
}

CRenderTarget* CRenderContext::GetRenderTarget()
{
    return m_RenderTarget;
}

CMatrixStack& CRenderContext::GetMatrixStack()
{
    return m_MatrixStack;
}



CPreRenderContext::CPreRenderContext(CRenderTarget* pRenderTarget) : m_RenderTarget(pRenderTarget)
{
}

CPreRenderContext::~CPreRenderContext()
{
}

CRenderTarget* CPreRenderContext::GetRenderTarget()
{
    return m_RenderTarget;
}

HRESULT CPreRenderContext::AddRenderRoot(IRenderRoot* pRoot)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoot);

    m_RenderRoots.push_back(pRoot);

Cleanup:
    return hr;
}

HRESULT CPreRenderContext::RenderRoots(CRenderContext& Context)
{
    HRESULT hr = S_OK;

    for(RenderRootCollection::reverse_iterator It = m_RenderRoots.rbegin(); It != m_RenderRoots.rend(); ++It)
    {
        IFC((*It)->RenderRoot(Context));
    }

Cleanup:
    return hr;
}