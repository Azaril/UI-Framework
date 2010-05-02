#include "Visual.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

CStaticProperty VisualProperties[] = 
{
    //TODO: Allow this to be 0 size!
    CStaticProperty( L"Width", TypeIndex::Float, StaticPropertyFlags::None )
};

namespace VisualPropertyIndex
{
    enum Value
    {
    };
}

CVisual::CVisual() : m_VisualAttached(FALSE)
{
    m_VisualTransform = D2D1::Matrix3x2F::Identity();
    m_FinalLocalTransform = D2D1::Matrix3x2F::Identity();
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
    CVisual* pVisual = NULL;

    IFCEXPECT(!m_VisualAttached);

    m_VisualAttached = TRUE;
    m_VisualContext = Context;

    for(UINT32 i = 0; i < GetVisualChildCount(); i++)
    {
        pVisual = GetVisualChild(i);

        IFC(pVisual->OnVisualAttach(Context));
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
    CVisual* pVisual = NULL;

    IFCEXPECT(m_VisualAttached);

    m_VisualAttached = FALSE;

    for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
    {
        IFC((*It)->OnVisualDetach(Context));
    }

    for(UINT32 i = 0; i < GetVisualChildCount(); i++)
    {
        CVisual* pVisual = GetVisualChild(i);

        IFC(pVisual->OnVisualDetach(Context));
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
                CVisualDetachContext VisualContext(this, m_VisualContext.GetGraphicsDevice());

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

    m_FinalLocalTransform = m_VisualTransform;

    IFC(MatrixStack.MultMatrixLocal(m_FinalLocalTransform));

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
                CVisualDetachContext VisualContext(this, m_VisualContext.GetGraphicsDevice());

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

UINT32 CVisual::GetVisualChildCount()
{
    return m_VisualChildren.size();
}

CVisual* CVisual::GetVisualChild(UINT32 Index)
{
    return m_VisualChildren.at(Index);
}

HRESULT CVisual::TransformToParent(CTransform** ppTransform)
{
    HRESULT hr = S_OK;
    CMatrixTransform* pMatrixTransform = NULL;
    Matrix3X2 TransformedMatrix = m_FinalLocalTransform;

    IFCPTR(ppTransform);

    //TODO: Remove D2D dependency!
    IFCEXPECT(D2D1IsMatrixInvertible(&m_FinalLocalTransform));

    IFCEXPECT(D2D1InvertMatrix(&TransformedMatrix));

    IFC(CMatrixTransform::Create(TransformedMatrix, &pMatrixTransform));

    *ppTransform = pMatrixTransform;
    pMatrixTransform = NULL;

Cleanup:
    ReleaseObject(pMatrixTransform);

    return hr;
}

HRESULT CVisual::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(VisualProperties, ARRAYSIZE(VisualProperties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

HRESULT CVisual::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= VisualProperties && pProperty < VisualProperties + ARRAYSIZE(VisualProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - VisualProperties);
        
        switch(Index)
        {
            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(E_FAIL);
    }

Cleanup:
    return hr;
}

HRESULT CVisual::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    // Check if the property is a static property.
    if(pProperty >= VisualProperties && pProperty < VisualProperties + ARRAYSIZE(VisualProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - VisualProperties);
        
        switch(Index)
        {
            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(E_FAIL);
    }

Cleanup:
    return hr;
}