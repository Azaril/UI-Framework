#include "Visual.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

CVisual::CVisual() : m_VisualAttached(FALSE)
{
    m_VisualTransform = Matrix3X2F::Identity();
    m_FinalLocalTransform = Matrix3X2F::Identity();
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

    {
        CVisualAttachContext VisualContext(this, m_VisualContext.GetGraphicsDevice());

        for(UINT32 i = 0; i < GetVisualChildCount(); i++)
        {
            pVisual = GetVisualChild(i);

            IFC(pVisual->OnVisualAttach(VisualContext));
        }

        for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
        {
            IFC((*It)->OnVisualAttach(VisualContext));
        }
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

    {
        CVisualDetachContext VisualContext(this, m_VisualContext.GetGraphicsDevice());

        for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
        {
            IFC((*It)->OnVisualDetach(VisualContext));
        }

        for(UINT32 i = 0; i < GetVisualChildCount(); i++)
        {
            CVisual* pVisual = GetVisualChild(i);

            IFC(pVisual->OnVisualDetach(VisualContext));
        }
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
        CVisualAttachContext VisualContext(this, m_VisualContext.GetGraphicsDevice());

        IFC(pVisualChild->OnVisualAttach(VisualContext));
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

    IFC(RenderChildren(Context));

Cleanup:
    return hr;
}

HRESULT CVisual::RenderChildren(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CMatrixStack& MatrixStack = Context.GetMatrixStack();

    const Matrix3X2F* pChildrenTransform = GetChildRenderTransform();

    if(pChildrenTransform)
    {
        IFC(MatrixStack.Push());

        IFC(MatrixStack.MultMatrixLocal(*pChildrenTransform));

        IFC(Context.GetRenderTarget()->SetTransform(MatrixStack.GetTop()));
    }

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        IFC((*It)->Render(Context));
    }

    if(pChildrenTransform)
    {
        IFC(MatrixStack.Pop());
    }

Cleanup:
    return hr;
}

HRESULT CVisual::SetVisualTransform(const Matrix3X2F& Matrix)
{
    HRESULT hr = S_OK;

    m_VisualTransform = Matrix;

    return hr;
}

const Matrix3X2F& CVisual::GetVisualTransform()
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
        CVisualAttachContext VisualContext(this, m_VisualContext.GetGraphicsDevice());

        IFC(pVisualResource->OnVisualAttach(VisualContext));
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

HRESULT CVisual::OnVisualNotification(CVisualNotification* pNotification)
{
    HRESULT hr = S_OK;

    IFCPTR(pNotification);

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

CVisual* CVisual::GetVisualParent()
{
    return m_VisualContext.GetParent();
}

const Matrix3X2F* CVisual::GetChildRenderTransform()
{
    return NULL;
}

HRESULT CVisual::TransformToParent(CTransform** ppTransform)
{
    HRESULT hr = S_OK;
    CMatrixTransform* pMatrixTransform = NULL;
    CVisual* pParent = GetVisualParent();

    IFCPTR(ppTransform);

    if(pParent)
    {
        IFC(TransformToAncestor(pParent, ppTransform));
    }
    else
    {
        IFC(CMatrixTransform::Create(m_FinalLocalTransform, &pMatrixTransform));

        *ppTransform = pMatrixTransform;
        pMatrixTransform = NULL;
    }

Cleanup:
    ReleaseObject(pMatrixTransform);

    return hr;
}

const Matrix3X2F& CVisual::GetFinalLocalTransform()
{
    return m_FinalLocalTransform;
}

HRESULT CVisual::TransformFromAncestor(CVisual* pAncestor, CTransform** ppTransform)
{
    HRESULT hr = S_OK;
    CVisual* pCurrentVisual = NULL;
    Matrix3X2F TransformMatrix = Matrix3X2F::Identity();
    CMatrixTransform* pMatrixTransform = NULL;

    pCurrentVisual = this;

    while(pCurrentVisual != NULL && pCurrentVisual != pAncestor)
    {
        TransformMatrix = pCurrentVisual->GetFinalLocalTransform() * TransformMatrix;

        pCurrentVisual = pCurrentVisual->GetVisualParent();

        if(pCurrentVisual)
        {
            const Matrix3X2F* pParentChildTransform = pCurrentVisual->GetChildRenderTransform();

            if(pParentChildTransform)
            {
                TransformMatrix = (*pParentChildTransform) * TransformMatrix;
            }
        }
    }

    IFCEXPECT(pAncestor == NULL || pCurrentVisual == pAncestor);

    IFC(CMatrixTransform::Create(TransformMatrix, &pMatrixTransform));

    *ppTransform = pMatrixTransform;
    pMatrixTransform = NULL;

Cleanup:
    return hr;
}

HRESULT CVisual::TransformToAncestor(CVisual* pAncestor, CTransform** ppTransform)
{
    HRESULT hr = S_OK;
    CTransform* pFromTransform = NULL;

    IFC(TransformFromAncestor(pAncestor, &pFromTransform));

    IFC(pFromTransform->Invert(ppTransform));

Cleanup:
    return hr;
}

HRESULT CVisual::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(NULL, 0, &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

HRESULT CVisual::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(CPropertyObject::SetValueInternal(pProperty, pValue));

Cleanup:
    return hr;
}

HRESULT CVisual::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    IFC(CPropertyObject::GetValue(pProperty, ppValue));

Cleanup:
    return hr;
}

//
// CVisual
//
extern "C" __declspec(dllexport)
TypeIndex::Value CVisual_TypeIndex()
{
    return TypeIndex::Visual;
}

extern "C" __declspec(dllexport)
CPropertyObject* CVisual_CastTo_CPropertyObject(CVisual* pVisual)
{
    return pVisual;
}

extern "C" __declspec(dllexport)
CVisual* CObjectWithType_CastTo_CVisual(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::Visual)) ? (CVisual*)pObject : NULL;
}