#include "BindingManager.h"

CBindingManager::CBindingManager()
{
}


CBindingManager::~CBindingManager()
{
}

HRESULT CBindingManager::Initialize()
{
    HRESULT hr = S_OK;

    return hr;
}

void CBindingManager::OnBindingInvalidated(CBindingBase* pBinding, SetValueFunc SetFunc)
{
    HRESULT hr = S_OK;

    IFC(SetValueFromBinding(pBinding, SetFunc));

Cleanup:
    ;
}

HRESULT CBindingManager::SetValueFromBinding(CBindingBase* pBinding, SetValueFunc SetFunc)
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

HRESULT CBindingManager::SetBinding(CPropertyObject* pTarget, CProperty* pTargetProperty, CBindingBase* pBinding, SetValueFunc SetFunc)
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

HRESULT CBindingManager::GetBindingContext(CPropertyObject* pTarget, CDefaultBindingContext** ppContext)
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



CDefaultBindingContext::CDefaultBindingContext()
{
}

CDefaultBindingContext::~CDefaultBindingContext()
{
    // Disconnect connections.
    for(std::vector< CPropertyBinding >::iterator It = m_Bindings.begin(); It != m_Bindings.end(); ++It)
    {
        It->GetBinding()->ClearTarget();

        It->Clear();
    }
}

HRESULT CDefaultBindingContext::Initialize()
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CDefaultBindingContext::SetPropertyBinding(CProperty* pProperty, CBindingBase* pBinding, SetValueFunc SetFunc, events::signals::connection ChangeConnection)
{
    HRESULT hr = S_OK;

    for(std::vector< CPropertyBinding >::iterator It = m_Bindings.begin(); It != m_Bindings.end(); ++It)
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