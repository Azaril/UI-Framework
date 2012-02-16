#include "FloatAnimation.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
namespace FloatAnimationproperties
{
    enum Value
    {
        From,
        To,
        Duration
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( From );
DEFINE_GET_DEFAULT_NULL( To );
DEFINE_GET_DEFAULT_NULL( Duration );

//
// Properties
//
CStaticProperty CFloatAnimation::FromProperty(FloatAnimationproperties::From, L"From", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( From ), NULL);
CStaticProperty CFloatAnimation::ToProperty(FloatAnimationproperties::To, L"To", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( To ), NULL);
CStaticProperty CFloatAnimation::DurationProperty(FloatAnimationproperties::Duration, L"Duration", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Duration ), NULL);

CFloatAnimation::CFloatAnimation(
    ) 
    : m_From(NULL)
    , m_To(NULL)
{
}

CFloatAnimation::~CFloatAnimation(
    )
{
    ReleaseObject(m_From);
    ReleaseObject(m_To);
    ReleaseObject(m_Duration);
}

__checkReturn HRESULT
CFloatAnimation::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT
CFloatAnimation::Initialize(
    FLOAT From,
    FLOAT To, 
    const CTimeSpan& Duration
    )
{
    HRESULT hr = S_OK;

    IFC(CFloatValue::Create(From, &m_From));
    IFC(CFloatValue::Create(To, &m_To));
    IFC(CDurationValue::Create(Duration, &m_Duration));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CFloatAnimation::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &FromProperty,
        &ToProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CFloatAnimationBase::CreatePropertyInformation(&pBaseInformation));
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
CFloatAnimation::SetValueInternal(
    __in CProperty* pProperty,
    __in CObjectWithType* pValue 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CFloatAnimation::FromProperty)
    {
        CFloatValue* pFrom = NULL;

        IFC(CastType(pValue, &pFrom));

        ReplaceObject(m_From, pFrom);
    }
    else if(pProperty == &CFloatAnimation::ToProperty)
    {
        CFloatValue* pTo = NULL;

        IFC(CastType(pValue, &pTo));

        ReplaceObject(m_To, pTo);
    }
    else
    {
        IFC(CFloatAnimationBase::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFloatAnimation::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CFloatAnimation::FromProperty)
    {
        SetObject(*ppValue, m_From);
    }
    else if(pProperty == &CFloatAnimation::ToProperty)
    {
        SetObject(*ppValue, m_To);
    }
    else
    {
        IFC(CFloatAnimationBase::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFloatAnimation::GetCurrentValue(
    __in CAnimationClock* pClock,
    __in CObjectWithType* pDefaultInitialValue, 
    __in CObjectWithType* pDefaultFinalValue,
    __deref_out CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    FLOAT Progress = 0;
    CFloatValue* pOutVal = NULL;

    CFloatValue* pFrom = NULL;
    CFloatValue* pTo = NULL;

    pFrom = m_From;
    pTo = m_To;
    
    if (pFrom == NULL)
    {
        IFC(CastType(pDefaultInitialValue, &pFrom));
    }
    
    if (pTo == NULL)
    {
        IFC(CastType(pDefaultFinalValue, &pTo));
    }

    if (pFrom != NULL && pTo != NULL)
    {
        FLOAT FromVal = pFrom->GetValue();
        FLOAT ToVal = pTo->GetValue();

        IFC(pClock->GetCurrentProgress(&Progress));

        FLOAT Range = ToVal - FromVal;

        FLOAT Value = FromVal + (Range * Progress);

        IFC(CFloatValue::Create(Value, &pOutVal));
    }
    
    *ppValue = pOutVal;
    pOutVal = NULL;

Cleanup:
    ReleaseObject(pOutVal);

    return hr;
}