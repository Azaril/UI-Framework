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
CStaticProperty CFloatAnimation::FromProperty(TypeIndex::FloatAnimation, FloatAnimationproperties::From, L"From", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( From ), NULL);
CStaticProperty CFloatAnimation::ToProperty(TypeIndex::FloatAnimation, FloatAnimationproperties::To, L"To", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( To ), NULL);
CStaticProperty CFloatAnimation::DurationProperty(TypeIndex::FloatAnimation, FloatAnimationproperties::Duration, L"Duration", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Duration ), NULL);

CFloatAnimation::CFloatAnimation(
    ) 
{
}

CFloatAnimation::~CFloatAnimation(
    )
{
}

__checkReturn HRESULT
CFloatAnimation::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CFloatAnimationBase::Initialize(pProviders));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFloatAnimation::Initialize(
    __in CProviders* pProviders,
    FLOAT From,
    FLOAT To, 
    const CTimeSpan& Duration
    )
{
    HRESULT hr = S_OK;
    CDurationValue* pDuration = NULL;

    IFC(Initialize(pProviders));

    IFC(CDurationValue::Create(Duration, &pDuration));

    IFC(SetValue(&FromProperty, From));
    IFC(SetValue(&FromProperty, To));
    IFC(SetValue(&FromProperty, pDuration));

Cleanup:
    ReleaseObject(pDuration);

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
CFloatAnimation::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::FloatAnimation)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case FloatAnimationproperties::From:
                {
                    *ppLayeredValue = &m_From;
                    break;
                }

            case FloatAnimationproperties::To:
                {
                    *ppLayeredValue = &m_To;
                    break;
                }

            case FloatAnimationproperties::Duration:
                {
                    *ppLayeredValue = &m_Duration;
                    break;
                }

            default:
                {
                    IFC(E_UNEXPECTED);
                }
        }
    }
    else
    {
        IFC_NOTRACE(CFloatAnimationBase::GetLayeredValue(pProperty, ppLayeredValue));
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

    IFC(GetTypedEffectiveValue(&FromProperty, &pFrom));
    IFC(GetTypedEffectiveValue(&ToProperty, &pTo));
    
    if (pFrom == NULL)
    {
        IFC(CastType(pDefaultInitialValue, &pFrom));
        AddRefObject(pFrom);
    }
    
    if (pTo == NULL)
    {
        IFC(CastType(pDefaultFinalValue, &pTo));
        AddRefObject(pTo);
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
    ReleaseObject(pFrom);
    ReleaseObject(pTo);

    return hr;
}