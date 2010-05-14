#include "Binding.h"
#include "StaticPropertyInformation.h"

CBinding::CBinding() : m_Target(NULL),
                       m_TargetProperty(NULL),
                       m_Providers(NULL)
{
}

CBinding::~CBinding()
{
    ReleaseObject(m_TargetProperty);
    ReleaseObject(m_Providers);
}

HRESULT CBinding::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

Cleanup:
    return hr;
}

HRESULT CBinding::CreatePropertyInformation(CPropertyInformation** ppInformation)
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

HRESULT CBinding::SetTarget(CPropertyObject* pTarget, CProperty* pTargetProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pTarget);
    IFCPTR(pTargetProperty);

    IFCEXPECT(m_Target == NULL);
    IFCEXPECT(m_TargetProperty == NULL);

    m_Target = pTarget;

    m_TargetProperty = pTargetProperty;
    AddRefObject(m_TargetProperty);

Cleanup:
    return hr;
}

HRESULT CBinding::ClearTarget()
{
    HRESULT hr = S_OK;

    m_Target = NULL;

    ReleaseObject(m_TargetProperty);

Cleanup:
    return hr;
}

HRESULT CBinding::AddChangeListener(const BindingInvalidatedHandler& Handler, connection* pConnection)
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_InvalidatedSignal.connect(Handler);

Cleanup:
    return hr;
}

HRESULT CBinding::InvalidateBinding()
{
    HRESULT hr = S_OK;

    m_InvalidatedSignal(this);

    return hr;
}