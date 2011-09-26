#include "Animatable.h"
#include "AnimationTimeline.h"

CAnimatable::CAnimatable(
    )
{
}

CAnimatable::~CAnimatable(
    )
{
}

__checkReturn HRESULT 
CAnimatable::BeginAnimation(
    __in CProperty* pProperty,
    __in CAnimationTimeline* pTimeline
    )
{
    HRESULT hr = S_OK;
    CAnimationClock* pClock = NULL;
    CTimeSource* pTimeSource = NULL;

    IFCPTR(pProperty);
    IFCPTR(pTimeline);

    IFCEXPECT(pTimeline->GetProvidedType() == pProperty->GetType());

    pTimeSource = GetTimeSource();
    IFCPTR(pTimeSource);

    IFC(pTimeline->CreateClock(pTimeSource, &pClock));

    IFC(ApplyAnimationClock(pProperty, pClock));

Cleanup:
    ReleaseObject(pClock);

    return hr;
}

__checkReturn HRESULT 
CAnimatable::ApplyAnimationClock(
    __in CProperty* pProperty,
    __in CAnimationClock* pClock
    )
{
    HRESULT hr = S_OK;
    CObjectWithType* pCurrentValue = NULL;
    CAnimationInfo* pAnimationInfo = NULL;
    events::signals::connection ChangeConnection;

    IFC(GetAnimationBaseValue(pProperty, &pCurrentValue));

    //TODO: Correctly handle hand-off behavior etc.

    for(std::vector< CAnimationInfo* >::iterator It = m_Animations.begin(); It != m_Animations.end(); ++It)
    {
        if((*It)->GetProperty() == pProperty && (*It)->GetClock() != pClock)
        {
            (*It)->Disconnect();

            (*It)->Release();

            m_Animations.erase(It);

            break;
        }
    }

    pAnimationInfo = new CAnimationInfo(pProperty, pCurrentValue, pClock);
    IFCOOM(pAnimationInfo);

    IFC(pClock->AddChangeListener(boost::bind(&CAnimatable::OnClockChanged, this, pAnimationInfo), &ChangeConnection));

    pAnimationInfo->SetConnection(ChangeConnection);

    m_Animations.push_back(pAnimationInfo);

    pAnimationInfo = NULL;

Cleanup:
    ReleaseObject(pCurrentValue);

    delete pAnimationInfo;

    if(FAILED(hr))
    {
        ChangeConnection.disconnect();
    }

    return hr;
}

void 
CAnimatable::OnClockChanged(
    __in CAnimationInfo* pInfo
    )
{
    HRESULT hr = S_OK;
    CAnimationClock* pClock = NULL;
    CObjectWithType* pValue = NULL;

    AddRefObject(pInfo);

    pClock = pInfo->GetClock();

    IFC(pClock->GetCurrentValue(pInfo->GetInitialValue(), pInfo->GetInitialValue(), &pValue));

    if (pValue != NULL)
    {
        IFC(SetAnimationValue(pInfo->GetProperty(), pValue));
    }

Cleanup:
    ReleaseObject(pInfo);
    ReleaseObject(pValue);
}

CAnimationInfo::CAnimationInfo( 
    __in CProperty* pProperty,
    __in CObjectWithType* pInitialValue,
    __in CAnimationClock* pClock 
    ) 
    : m_Property(pProperty)
    , m_InitialValue(pInitialValue)
    , m_Clock(pClock)
{
    AddRefObject(m_Property);
    AddRefObject(m_Clock);
    AddRefObject(m_InitialValue);
}

CAnimationInfo::~CAnimationInfo(
    )
{
    ReleaseObject(m_Property);
    ReleaseObject(m_Clock);
    ReleaseObject(m_InitialValue);

    m_ChangeConnection.disconnect();
}

__out CProperty*
CAnimationInfo::GetProperty(
    )
{
    return m_Property;
}

void 
CAnimationInfo::SetProperty(
    __in CProperty* pProperty
    )
{
    ReleaseObject(m_Property);

    m_Property = pProperty;

    AddRefObject(m_Property);
}

__out CAnimationClock*
CAnimationInfo::GetClock(
    )
{
    return m_Clock;
}

void
CAnimationInfo::SetClock(
    __in CAnimationClock* pClock
    )
{
    SetObject(m_Clock, pClock);
}

void 
CAnimationInfo::SetConnection(
    events::signals::connection Connection
    )
{
    m_ChangeConnection = Connection;
}

void 
CAnimationInfo::Disconnect(
    )
{
    m_ChangeConnection.disconnect();
}

__out CObjectWithType*
CAnimationInfo::GetInitialValue(
    )
{
    return m_InitialValue;
}