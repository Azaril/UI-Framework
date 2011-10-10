#include "BindingManager.h"

CBindingManager::CBindingManager(
    )
{
}

CBindingManager::~CBindingManager(
    )
{
}

__checkReturn HRESULT 
CBindingManager::Initialize(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

void 
CBindingManager::OnBindingInvalidated(
    __in CBindingBase* pBinding, 
    __in SetValueFunc SetFunc
    )
{
    HRESULT hr = S_OK;

    IFC(SetValueFromBinding(pBinding, SetFunc));

Cleanup:
    ;
}

__checkReturn HRESULT 
CBindingManager::SetValueFromBinding(
    __in CBindingBase* pBinding, 
    __in SetValueFunc SetFunc
    )
{
    HRESULT hr = S_OK;
    CObjectWithType* pValue = NULL;

    if(SUCCEEDED(pBinding->GetBoundValue(&pValue)) && pValue != NULL)
    {
        IFC(SetFunc(pBinding->GetTarget(), pBinding->GetTargetProperty(), pValue));
    }
    else
    {
        //TODO: Clear value.
    }

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

__checkReturn HRESULT 
CBindingManager::SetBinding(
    __in CPropertyObject* pTarget, 
    __in CProperty* pTargetProperty, 
    __in CBindingBase* pBinding, 
    __in SetValueFunc SetFunc
    )
{
    HRESULT hr = S_OK;
    CDefaultBindingContext* pBindingContext = NULL;
    events::signals::connection InvalidationConnection;

    IFCPTR(pTarget);
    IFCPTR(pTargetProperty);
    IFCPTR(pBinding);

    IFC(GetBindingContext(pTarget, &pBindingContext));

    IFC(pBinding->SetTarget(pTarget, pTargetProperty));

    IFC(pBinding->AddChangeListener(bind(&CBindingManager::OnBindingInvalidated, _1, SetFunc), &InvalidationConnection));

    IFC(pBindingContext->SetPropertyBinding(pTargetProperty, pBinding, SetFunc, InvalidationConnection));

    IFC(SetValueFromBinding(pBinding, SetFunc));

Cleanup:
    ReleaseObject(pBindingContext);

    return hr;
}

__checkReturn HRESULT
CBindingManager::GetBindingContext(
    __in CPropertyObject* pTarget, 
    __deref_out CDefaultBindingContext** ppContext
    )
{
    HRESULT hr = S_OK;
    CBindingContext* pContext = NULL;
    CDefaultBindingContext* pBindingContext = NULL;

    IFC(pTarget->GetBindingContext(&pContext));

    if(pContext == NULL)
    {
        IFC(CDefaultBindingContext::Create(&pBindingContext));

        IFC(pTarget->SetBindingContext(pBindingContext));
    }
    else
    {
        pBindingContext = (CDefaultBindingContext*)pContext;
        pContext = NULL;
    }

    *ppContext = pBindingContext;
    pBindingContext = NULL;

Cleanup:
    ReleaseObject(pContext);
    ReleaseObject(pBindingContext);

    return hr;
}

CDefaultBindingContext::CDefaultBindingContext(
    )
{
}

CDefaultBindingContext::~CDefaultBindingContext(
    )
{
    // Disconnect connections.
    for(vector< CPropertyBinding >::iterator It = m_Bindings.begin(); It != m_Bindings.end(); ++It)
    {
        IGNOREHR(It->GetBinding()->ClearTarget());

        It->Clear();
    }
}

__checkReturn HRESULT 
CDefaultBindingContext::Initialize(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CDefaultBindingContext::SetPropertyBinding(
    __in CProperty* pProperty, 
    __in CBindingBase* pBinding, 
    __in SetValueFunc SetFunc, 
    events::signals::connection ChangeConnection
    )
{
    HRESULT hr = S_OK;

    for(vector< CPropertyBinding >::iterator It = m_Bindings.begin(); It != m_Bindings.end(); ++It)
    {
        if(It->GetProperty() == pProperty && It->GetSetFunction() == SetFunc)
        {
            IFC(It->GetBinding()->ClearTarget());

            It->Clear();

            m_Bindings.erase(It);

            break;
        }
    }

    m_Bindings.push_back(CPropertyBinding(pProperty, pBinding, SetFunc, ChangeConnection));

Cleanup:
    return hr;
}