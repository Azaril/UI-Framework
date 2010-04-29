#include "Visual.h"

CVisual::CVisual() : m_VisualAttached(FALSE)
{
    m_VisualTransform = D2D1::Matrix3x2F::Identity();
}

CVisual::~CVisual()
{
    Finalize();
}

HRESULT CVisual::Initialize()
{
    HRESULT hr = S_OK;
    
    return hr;
}

HRESULT CVisual::Finalize()
{
    HRESULT hr = S_OK;

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        (*It)->Release();
    }

    for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
    {
        (*It)->Release();
    }

    m_VisualChildren.clear();

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

    for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
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

    for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
    {
        IFC((*It)->OnVisualDetach(Context));
    }

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
                CVisualDetachContext VisualContext(m_VisualContext.GetGraphicsDevice());

                IFC(pVisualChild->OnVisualDetach(VisualContext));
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

HRESULT CVisual::AddVisualResource(CVisualResource* pVisualResource)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualResource);

    for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
    {
        if(*It == pVisualResource)
        {
            IFC(E_FAIL);
        }
    }

    m_VisualResources.push_back(pVisualResource);

    AddRefObject(pVisualResource);

    if(m_VisualAttached)
    {
        IFC(pVisualResource->OnVisualAttach(m_VisualContext));
    }

Cleanup:
    return hr;
}

HRESULT CVisual::RemoveVisualResource(CVisualResource* pVisualResource)
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualResource);

    for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
    {
        if(*It == pVisualResource)
        {
            if(m_VisualAttached)
            {
                CVisualDetachContext VisualContext(m_VisualContext.GetGraphicsDevice());

                IFC(pVisualResource->OnVisualDetach(VisualContext));
            }

            (*It)->Release();

            m_VisualResources.erase(It);
            
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}