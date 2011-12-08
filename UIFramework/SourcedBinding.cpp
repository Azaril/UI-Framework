#include "SourcedBinding.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT( BindingDirection, BindingDirection::Value, BindingDirection::OneWay );

//
// Properties
//
CStaticProperty CSourcedBinding::BindingDirectionProperty(L"BindingDirection", TypeIndex::BindingDirection, StaticPropertyFlags::None, &GET_DEFAULT( BindingDirection ), &INSTANCE_CHANGE_CALLBACK( CSourcedBinding, OnBindingDirectionChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CSourcedBinding, OnBindingDirectionChanged );

CSourcedBinding::CSourcedBinding(
    ) 
    : m_Source(NULL)
    , m_SourceProperty(NULL)
    , m_BindingDirection(BindingDirection::OneWay)
{
}

CSourcedBinding::~CSourcedBinding(
    )
{
    m_SourcePropertyChangedConnection.disconnect();
    m_TargetPropertyChangedConnection.disconnect();

    ReleaseObject(m_Source);
    ReleaseObject(m_SourceProperty);
}

__checkReturn HRESULT 
CSourcedBinding::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CBindingBase::Initialize(pProviders));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CSourcedBinding::SetSource(
    __in CPropertyObject* pSource,
    __in CProperty* pSourceProperty
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSource);
    IFCPTR(pSourceProperty);

    m_Source = pSource;
    AddRefObject(m_Source);

    m_SourceProperty = pSourceProperty;
    AddRefObject(m_SourceProperty);

    IFC(pSource->AddPropertyChangeListener(bind(&CSourcedBinding::OnSourcePropertyChanged, this, _1, _2), &m_SourcePropertyChangedConnection));

    IFC(InvalidateBinding());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CSourcedBinding::ClearSource(
    )
{
    HRESULT hr = S_OK;

    m_SourcePropertyChangedConnection.disconnect();

    if(m_Source != NULL && m_SourceProperty != NULL)
    {
        ReleaseObject(m_Source);
        ReleaseObject(m_SourceProperty);

        IFC(InvalidateBinding());
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CSourcedBinding::SetTarget(
    __in CPropertyObject* pTarget,
    __in CProperty* pTargetProperty
    )
{
    HRESULT hr = S_OK;

    IFC(CBindingBase::SetTarget(pTarget, pTargetProperty));

    if(m_BindingDirection == BindingDirection::TwoWay)
    {
        IFC(pTarget->AddPropertyChangeListener(bind(&CSourcedBinding::OnTargetPropertyChanged, this, _1, _2), &m_TargetPropertyChangedConnection));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CSourcedBinding::ClearTarget(
    )
{
    HRESULT hr = S_OK;

    IFC(CBindingBase::ClearTarget());

    m_TargetPropertyChangedConnection.disconnect();

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CSourcedBinding::GetBoundValue(
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    CObjectWithType* pEffectiveValue = NULL;
    CPropertyObject* pTarget = NULL;
    CProperty* pTargetProperty = NULL;

    IFCPTR(ppValue);

    pTarget = GetTarget();
    IFCPTR(pTarget);

    pTargetProperty = GetTargetProperty();
    IFCPTR(pTargetProperty);

    IFCPTR_NOTRACE(m_Source);
    IFCPTR(m_SourceProperty);

    IFC(m_Source->GetEffectiveValue(m_SourceProperty, &pEffectiveValue));

    *ppValue = pEffectiveValue;
    pEffectiveValue = NULL;
    
Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

void 
CSourcedBinding::OnSourcePropertyChanged(
    __in CPropertyObject* pObject, 
    __in CProperty* pProperty
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);
    IFCPTR(pProperty);

    if(pProperty == m_SourceProperty)
    {
        IFC(InvalidateBinding());
    }

Cleanup:
    ;
}

void 
CSourcedBinding::OnTargetPropertyChanged(
    __in CPropertyObject* pObject, 
    __in CProperty* pProperty
    )
{
    HRESULT hr = S_OK;
    CObjectWithType* pValue = NULL;
    CProperty* pTargetProperty = GetTargetProperty();

    IFCPTR(pObject);
    IFCPTR(pProperty);

    if(pTargetProperty != NULL && pProperty == GetTargetProperty())
    {
        CPropertyObject* pTarget = GetTarget();

        IFCPTR(m_Source);
        IFCPTR(m_SourceProperty);

        IFCPTR(pTarget);

        IFC(pTarget->GetEffectiveValue(pTargetProperty, &pValue));

        IFC(m_Source->SetEffectiveValue(m_SourceProperty, pValue));
    }

Cleanup:
    ;
}

__checkReturn HRESULT
CSourcedBinding::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &BindingDirectionProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CBindingBase::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

__override __checkReturn HRESULT
CSourcedBinding::SetValueInternal(
    __in CProperty* pProperty,
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;
    CBindingDirectionValue* pBindingDirection = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CSourcedBinding::BindingDirectionProperty)
    {
        IFCPTR(pValue);

        IFC(CastType(pValue, &pBindingDirection));

        m_BindingDirection = pBindingDirection->GetValue();
    }
    else
    {
        IFC(CBindingBase::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CSourcedBinding::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    CBindingDirectionValue* pBindingDirection = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CSourcedBinding::BindingDirectionProperty)
    {
        IFC(CBindingDirectionValue::Create(m_BindingDirection, &pBindingDirection));

        *ppValue = pBindingDirection;
        pBindingDirection = NULL;
    }
    else
    {
        IFC(CBindingBase::GetValue(pProperty, ppValue));
    }

Cleanup:
    ReleaseObject(pBindingDirection);

    return hr;
}

__checkReturn HRESULT 
CSourcedBinding::OnBindingDirectionChanged(
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;
    CBindingDirectionValue* pBindingDirection = NULL;

    IFCPTR(pNewValue);

    IFC(CastType(pNewValue, &pBindingDirection));

    if(pBindingDirection->GetValue() == BindingDirection::TwoWay)
    {
        CPropertyObject* pTarget = GetTarget();

        if(pTarget != NULL && !m_TargetPropertyChangedConnection.connected())
        {
            IFC(pTarget->AddPropertyChangeListener(bind(&CSourcedBinding::OnTargetPropertyChanged, this, _1, _2), &m_TargetPropertyChangedConnection));
        }
    }
    else
    {
        m_TargetPropertyChangedConnection.disconnect();
    }

Cleanup:
    return hr;
}