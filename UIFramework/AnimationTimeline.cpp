#include "AnimationTimeline.h"

//
// Properties
//
namespace AnimationTimelineProperties
{
    enum Value
    {
        Duration
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Duration );

//
// Properties
//
CStaticProperty CAnimationTimeline::DurationProperty(TypeIndex::AnimationTimeline, AnimationTimelineProperties::Duration, L"Duration", TypeIndex::Duration, StaticPropertyFlags::None, &GET_DEFAULT( Duration ), NULL);

CAnimationTimeline::CAnimationTimeline(
    )
{
}

CAnimationTimeline::~CAnimationTimeline(
    )
{
}

__checkReturn HRESULT 
CAnimationTimeline::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &DurationProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

__checkReturn HRESULT 
CAnimationTimeline::CreateClock(
    __in CTimeSource* pTimeSource,
    __deref_out CAnimationClock** ppClock
    )
{
    HRESULT hr = S_OK;
    events::signals::connection CompletedConnection;
    CAnimationClock* pClock = NULL;

    IFC(CAnimationClock::Create(pTimeSource, this, &pClock));

    IFC(pClock->AddCompletedListener(bind(&CAnimationTimeline::OnClockCompleted, this, _1), &CompletedConnection));

    *ppClock = pClock;
    pClock = NULL;

Cleanup:
    ReleaseObject(pClock);

    return hr;
}

void 
CAnimationTimeline::OnClockCompleted(
    __in CAnimationClock* pClock
    )
{
    m_Completed(this, pClock);
}

__checkReturn HRESULT 
CAnimationTimeline::AddCompletedListener(
    const TimelineCompletedHandler& Handler, 
    __out events::signals::connection* pConnection
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_Completed.connect(Handler);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CAnimationTimeline::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::AnimationTimeline)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case AnimationTimelineProperties::Duration:
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
        IFC_NOTRACE(CPropertyObject::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}