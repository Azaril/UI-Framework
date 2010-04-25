#include "Visual.h"

CVisual::CVisual() : m_VisualAttached(FALSE)
{
    m_VisualTransform = D2D1::Matrix3x2F::Identity();
}

CVisual::~CVisual()
{
}

HRESULT CVisual::Initialize()
{
    HRESULT hr = S_OK;
    
    return hr;
}

HRESULT CVisual::OnVisualAttach(CVisualAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFCEXPECT(!m_VisualAttached);

    m_VisualAttached = TRUE;
    m_VisualContext = Context;

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        IFC((*It)->OnVisualAttach(Context));
    }
    
Cleanup:
    return hr;
}

HRESULT CVisual::OnVisualDetach(CVisualDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_VisualAttached);

    m_VisualAttached = FALSE;

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        IFC((*It)->OnVisualDetach(Context));
    }

    m_VisualContext.Reset();

Cleanup:
    return hr;
}

HRESULT CVisual::AddChildVisual(CVisual* pVisualChild)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualChild);

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        if(*It == pVisualChild)
        {
            IFC(E_FAIL);
        }
    }

    m_VisualChildren.push_back(pVisualChild);

    AddRefObject(pVisualChild);

    if(m_VisualAttached)
    {
        IFC(pVisualChild->OnVisualAttach(m_VisualContext));
    }

Cleanup:
    return hr;
}

HRESULT CVisual::RemoveChildVisual(CVisual* pVisualChild)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualChild);

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        if(*It == pVisualChild)
        {
            if(m_VisualAttached)
            {
                CVisualDetachContext Context;

                IFC(pVisualChild->OnVisualDetach(Context));
            }

            (*It)->Release();

            m_VisualChildren.erase(It);
            
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CVisual::MoveToBack(CVisual* pVisualChild)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualChild);

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        if(*It == pVisualChild)
        {
            m_VisualChildren.erase(It);

            m_VisualChildren.insert(m_VisualChildren.begin(), pVisualChild);
            
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CVisual::MoveToFront(CVisual* pVisualChild)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualChild);

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        if(*It == pVisualChild)
        {
            m_VisualChildren.erase(It);

            m_VisualChildren.push_back(pVisualChild);
            
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CVisual::PreRender(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        IFC((*It)->PreRender(Context));
    }

Cleanup:
    return hr;
}

HRESULT CVisual::Render(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CMatrixStack& MatrixStack = Context.GetMatrixStack();

    IFC(MatrixStack.Push());

    IFC(MatrixStack.MultMatrixLocal(m_VisualTransform));

    IFC(Context.GetRenderTarget()->SetTransform(MatrixStack.GetTop()));

    IFC(RenderTransformed(Context));

    IFC(MatrixStack.Pop());

    IFC(Context.GetRenderTarget()->SetTransform(MatrixStack.GetTop()));

Cleanup:
    return hr;
}

HRESULT CVisual::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        IFC((*It)->Render(Context));
    }

Cleanup:
    return hr;
}

HRESULT CVisual::SetVisualTransform(const Matrix3X2& Matrix)
{
    HRESULT hr = S_OK;

    m_VisualTransform = Matrix;

    return hr;
}

const Matrix3X2& CVisual::GetVisualTransform()
{
    return m_VisualTransform;
}