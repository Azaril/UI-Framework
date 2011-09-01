#include "AnimationTimeline.h"

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