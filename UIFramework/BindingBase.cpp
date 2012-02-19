#include "BindingBase.h"
#include "StaticPropertyInformation.h"

CBindingBase::CBindingBase(
    ) 
    : m_Target(NULL)
    , m_TargetProperty(NULL)
{
}

CBindingBase::~CBindingBase(
    )
{
    ReleaseObject(m_TargetProperty);
}

__checkReturn HRESULT 
CBindingBase::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CPropertyObject::Initialize(pProviders));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CBindingBase::CreatePropertyInformation(
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

__checkReturn HRESULT 
CBindingBase::SetTarget(
    __in CPropertyObject* pTarget,
    __in CProperty* pTargetProperty
    )
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

__checkReturn HRESULT 
CBindingBase::ClearTarget(
    )
{
    HRESULT hr = S_OK;

    m_Target = NULL;

    ReleaseObject(m_TargetProperty);

    return hr;
}

__out_opt CPropertyObject* 
CBindingBase::GetTarget(
    )
{
    return m_Target;
}

__out_opt CProperty* 
CBindingBase::GetTargetProperty(
    )
{
    return m_TargetProperty;
}

__checkReturn HRESULT 
CBindingBase::AddChangeListener(
    const BindingInvalidatedHandler& Handler, 
    __out events::signals::connection* pConnection
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_InvalidatedSignal.connect(Handler);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CBindingBase::InvalidateBinding(
    )
{
    HRESULT hr = S_OK;

    m_InvalidatedSignal(this);

    return hr;
}