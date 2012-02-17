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
    : m_Duration(NULL)
{
}

CAnimationTimeline::~CAnimationTimeline(
    )
{
    ReleaseObject(m_Duration);
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

__checkReturn HRESULT 
CAnimationTimeline::GetDuration(
    __deref_out CDurationValue** ppDuration
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppDuration);

    *ppDuration = m_Duration;
    AddRefObject(m_Duration);

Cleanup:
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
CAnimationTimeline::SetValueInternal(
    __in CProperty* pProperty,
    __in CObjectWithType* pValue 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CAnimationTimeline::DurationProperty)
    {
        IFC(CastType(pValue, &m_Duration));

        AddRefObject(m_Duration);
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CAnimationTimeline::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CAnimationTimeline::DurationProperty)
    {
        *ppValue = m_Duration;
        AddRefObject(m_Duration);
    }
    else
    {
        IFC(CPropertyObject::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}