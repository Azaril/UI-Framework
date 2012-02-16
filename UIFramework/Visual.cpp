#include "Visual.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CVisual, OnChildInvalidated );

CVisual::CVisual(
    ) 
    : m_VisualAttached(FALSE)
{
}

CVisual::~CVisual(
    )
{
    Finalize();
}

__checkReturn HRESULT 
CVisual::Initialize(
    )
{
    HRESULT hr = S_OK;
    
    return hr;
}

__checkReturn HRESULT 
CVisual::Finalize(
    )
{
    HRESULT hr = S_OK;

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        (*It)->Release();
    }

    for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
    {
        It->first->Release();
    }

    m_VisualChildren.clear();

    return hr;
}

__checkReturn HRESULT 
CVisual::OnVisualAttach(
    CVisualAttachContext& Context
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(!m_VisualAttached);

    m_VisualAttached = TRUE;
    m_VisualContext = Context;

    {
        for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
        {
            CVisualAttachContext VisualContext(this, &INSTANCE_CHANGE_CALLBACK( CVisual, OnChildInvalidated ), Context.GetTimeSource(), Context.GetGraphicsDevice());

            IFC((*It)->OnVisualAttach(VisualContext));
        }

        for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
        {
            CVisualAttachContext VisualContext(this, It->second, Context.GetTimeSource(), Context.GetGraphicsDevice());

            IFC(It->first->OnVisualAttach(VisualContext));
        }
    }

    if (Context.GetParent() != NULL && Context.GetChangeCallback() != NULL)
    {
        IFC(Context.GetChangeCallback()(Context.GetParent(), this, NULL));
    }
    
Cleanup:
    return hr;
}

__checkReturn HRESULT
CVisual::OnVisualDetach(
    CVisualDetachContext& Context
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_VisualAttached);

    m_VisualAttached = FALSE;

    {
        for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
        {
            CVisualDetachContext VisualContext(this, &INSTANCE_CHANGE_CALLBACK( CVisual, OnChildInvalidated ), Context.GetGraphicsDevice());

            IFC((*It)->OnVisualDetach(VisualContext));
        }

        for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
        {
            CVisualDetachContext VisualContext(this, It->second, Context.GetGraphicsDevice());

            IFC(It->first->OnVisualDetach(VisualContext));
        }
    }

    if (Context.GetParent() != NULL && Context.GetChangeCallback() != NULL)
    {
        IFC(Context.GetChangeCallback()(Context.GetParent(), this, NULL));
    }

    m_VisualContext.Reset();

Cleanup:
    return hr;
}

__override __out_opt CTimeSource* 
CVisual::GetTimeSource(
    )
{
    return m_VisualContext.GetTimeSource();
}

__override __checkReturn HRESULT 
CVisual::SetAnimationValue(
    __in CProperty* pProperty,
    __in CObjectWithType* pValue 
    )
{
    HRESULT hr = S_OK;

    IFC(SetValue(pProperty, pValue));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CVisual::GetAnimationBaseValue(
    __in CProperty* pProperty,
    __deref_out CObjectWithType** ppValue 
    )
{
    HRESULT hr = S_OK;

    IFC(GetEffectiveValue(pProperty, ppValue));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::AddChildVisual(
    __in CVisual* pVisualChild
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualChild);

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        if((*It) == pVisualChild)
        {
            IFC(E_FAIL);
        }
    }

    m_VisualChildren.push_back(pVisualChild);

    AddRefObject(pVisualChild);

    if(m_VisualAttached)
    {
        CVisualAttachContext VisualContext(this, &INSTANCE_CHANGE_CALLBACK( CVisual, OnChildInvalidated ), m_VisualContext.GetTimeSource(), m_VisualContext.GetGraphicsDevice());

        IFC(pVisualChild->OnVisualAttach(VisualContext));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::RemoveChildVisual(
    __in CVisual* pVisualChild
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualChild);

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        if((*It) == pVisualChild)
        {
            if(m_VisualAttached)
            {
                CVisualDetachContext VisualContext(this, &INSTANCE_CHANGE_CALLBACK( CVisual, OnChildInvalidated ), m_VisualContext.GetGraphicsDevice());

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

__checkReturn HRESULT 
CVisual::MoveToBack(
    __in CVisual* pVisualChild
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualChild);

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        if((*It) == pVisualChild)
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

__checkReturn HRESULT 
CVisual::MoveToFront(
    __in CVisual* pVisualChild
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualChild);

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        if((*It) == pVisualChild)
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

__checkReturn HRESULT 
CVisual::PreRender(
    CPreRenderContext& Context
    )
{
    HRESULT hr = S_OK;

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        IFC((*It)->PreRender(Context));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::Render(
    CRenderContext& Context
    )
{
    HRESULT hr = S_OK;
    Matrix3X2F LocalTransform;
    CMatrixStack& MatrixStack = Context.GetMatrixStack();

    IFC(MatrixStack.Push());

    IFC(GetLocalTransform(&LocalTransform));

    IFC(MatrixStack.MultMatrixLocal(LocalTransform));

    IFC(Context.GetRenderTarget()->SetTransform(MatrixStack.GetTop()));

    IFC(RenderTransformed(Context));

    IFC(MatrixStack.Pop());

    IFC(Context.GetRenderTarget()->SetTransform(MatrixStack.GetTop()));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CVisual::RenderTransformed(
    CRenderContext& Context
    )
{
    HRESULT hr = S_OK;

    IFC(RenderChildren(Context));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::RenderChildren(
    CRenderContext& Context
    )
{
    HRESULT hr = S_OK;
    CMatrixStack& MatrixStack = Context.GetMatrixStack();

    const Matrix3X2F* pChildrenTransform = GetChildRenderTransform();

    if(pChildrenTransform != NULL)
    {
        IFC(MatrixStack.Push());

        IFC(MatrixStack.MultMatrixLocal(*pChildrenTransform));

        IFC(Context.GetRenderTarget()->SetTransform(MatrixStack.GetTop()));
    }

    for(VisualChildCollection::iterator It = m_VisualChildren.begin(); It != m_VisualChildren.end(); ++It)
    {
        IFC((*It)->Render(Context));
    }

    if(pChildrenTransform != NULL)
    {
        IFC(MatrixStack.Pop());
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::GetLocalTransform(
    __out Matrix3X2F* pTransform
    )
{
    HRESULT hr = S_OK;

    *pTransform = Matrix3X2F::Identity();

    return hr;
}

__checkReturn HRESULT 
CVisual::AddVisualResource(
    __in CVisualResource* pVisualResource, 
    __in_opt CProperty* pProperty
    )
{
    HRESULT hr = S_OK;

    IFC(AddVisualResource(pVisualResource, pProperty != NULL ? pProperty->GetOnValueChangedCallback() : NULL));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::AddVisualResource(
    __in CVisualResource* pVisualResource, 
    __in_opt OnValueChangeFunc ChangeFunc
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualResource);

    m_VisualResources.push_back(std::pair< CVisualResource*, OnValueChangeFunc >(pVisualResource, ChangeFunc));

    AddRefObject(pVisualResource);

    if(m_VisualAttached)
    {
        CVisualAttachContext VisualContext(this, ChangeFunc, m_VisualContext.GetTimeSource(), m_VisualContext.GetGraphicsDevice());

        IFC(pVisualResource->OnVisualAttach(VisualContext));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::RemoveVisualResource(
    __in CVisualResource* pVisualResource, 
    __in_opt CProperty* pProperty
    )
{
    HRESULT hr = S_OK;

    IFC(RemoveVisualResource(pVisualResource, pProperty != NULL ? pProperty->GetOnValueChangedCallback() : NULL));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::RemoveVisualResource(
    __in CVisualResource* pVisualResource, 
    __in_opt OnValueChangeFunc ChangeFunc
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pVisualResource);

    for(VisualResourceCollection::iterator It = m_VisualResources.begin(); It != m_VisualResources.end(); ++It)
    {
        if(It->first == pVisualResource && It->second == ChangeFunc)
        {
            if(m_VisualAttached)
            { 
                CVisualDetachContext VisualContext(this, ChangeFunc, m_VisualContext.GetGraphicsDevice());

                IFC(pVisualResource->OnVisualDetach(VisualContext));
            }

            It->first->Release();

            m_VisualResources.erase(It);
            
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

UINT32 
CVisual::GetVisualChildCount(
    )
{
    return m_VisualChildren.size();
}

__out CVisual* 
CVisual::GetVisualChild(
    UINT32 Index
    )
{
    return m_VisualChildren.at(Index);
}

__out_opt CVisual* 
CVisual::GetVisualParent(
    )
{
    return m_VisualContext.GetParent();
}

__out_opt const Matrix3X2F* 
CVisual::GetChildRenderTransform(
    )
{
    return NULL;
}

__checkReturn HRESULT
CVisual::TransformToParent(
    __deref_out CTransform** ppTransform
    )
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
        IFC(CMatrixTransform::Create(Matrix3X2F::Identity(), &pMatrixTransform));

        *ppTransform = pMatrixTransform;
        pMatrixTransform = NULL;
    }

Cleanup:
    ReleaseObject(pMatrixTransform);

    return hr;
}

__checkReturn HRESULT 
CVisual::TransformFromAncestor(
    __in CVisual* pAncestor, 
    __deref_out CTransform** ppTransform
    )
{
    HRESULT hr = S_OK;
    CVisual* pCurrentVisual = NULL;
    Matrix3X2F TransformMatrix = Matrix3X2F::Identity();
    CMatrixTransform* pMatrixTransform = NULL;

    pCurrentVisual = this;

    while(pCurrentVisual != NULL && pCurrentVisual != pAncestor)
    {
        Matrix3X2F LocalTransform;

        IFC(pCurrentVisual->GetLocalTransform(&LocalTransform));

        TransformMatrix = LocalTransform * TransformMatrix;

        pCurrentVisual = pCurrentVisual->GetVisualParent();

        if(pCurrentVisual != NULL && pCurrentVisual != pAncestor)
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

__checkReturn HRESULT 
CVisual::TransformToAncestor(
    __in CVisual* pAncestor,
    __deref_out CTransform** ppTransform
    )
{
    HRESULT hr = S_OK;
    CTransform* pFromTransform = NULL;

    IFC(TransformFromAncestor(pAncestor, &pFromTransform));

    IFC(pFromTransform->Invert(ppTransform));

Cleanup:
    ReleaseObject(pFromTransform);

    return hr;
}

__checkReturn HRESULT 
CVisual::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
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

__override __checkReturn HRESULT 
CVisual::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(CPropertyObject::SetValueInternal(pProperty, pValue));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    IFC(CPropertyObject::GetValueInternal(pProperty, ppValue));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CVisual::InvalidateVisual(
    )
{
    HRESULT hr = S_OK;
    CVisual* pParent = NULL;

    pParent = GetVisualParent();

    if (pParent != NULL)
    {
        if (m_VisualContext.GetChangeCallback() != NULL)
        {
            IFC(m_VisualContext.GetChangeCallback()(pParent, this, this));
        }
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisual::OnChildInvalidated(
    __in CObjectWithType* pOldValue,
    __in CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    //TODO: Add dirtines propagation
    //IFC(InvalidateVisual());

    return hr;
}

//
// CVisual
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value 
CVisual_TypeIndex(
    )
{
    return TypeIndex::Visual;
}

extern "C" UIFRAMEWORK_API
__out CPropertyObject*
CVisual_CastTo_CPropertyObject(
    __in CVisual* pVisual
    )
{
    return pVisual;
}

extern "C" UIFRAMEWORK_API
__out_opt CVisual*
CObjectWithType_CastTo_CVisual(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::Visual)) ? (CVisual*)pObject : NULL;
}