#include "BindingBase.h"
#include "StaticPropertyInformation.h"

CBindingBase::CBindingBase() : m_Target(NULL),
                               m_TargetProperty(NULL),
                               m_Providers(NULL)
{
}

CBindingBase::~CBindingBase()
{
    ReleaseObject(m_TargetProperty);
    ReleaseObject(m_Providers);
}

HRESULT CBindingBase::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

Cleanup:
    return hr;
}

HRESULT CBindingBase::CreatePropertyInformation(CPropertyInformation** ppInformation)
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

HRESULT CBindingBase::SetTarget(CPropertyObject* pTarget, CProperty* pTargetProperty)
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

HRESULT CBindingBase::ClearTarget()
{
    HRESULT hr = S_OK;

    m_Target = NULL;

    ReleaseObject(m_TargetProperty);

    return hr;
}

CPropertyObject* CBindingBase::GetTarget()
{
    return m_Target;
}

CProperty* CBindingBase::GetTargetProperty()
{
    return m_TargetProperty;
}

HRESULT CBindingBase::AddChangeListener(const BindingInvalidatedHandler& Handler, events::signals::connection* pConnection)
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_InvalidatedSignal.connect(Handler);

Cleanup:
    return hr;
}

HRESULT CBindingBase::InvalidateBinding()
{
    HRESULT hr = S_OK;

    m_InvalidatedSignal(this);

    return hr;
}