#include "AnimationClock.h"
#include "AnimationTimeline.h"
#include "BasicTypes.h"

CAnimationClock::CAnimationClock(
    ) 
    : m_RunningDuration(CTimeSpan::Zero())
    , m_Timeline(NULL)
    , m_TimeSource(NULL)
    , m_Connected(FALSE)
    , m_ClockState(ClockState::Active)
    , m_Progress(0)
{
}

CAnimationClock::~CAnimationClock(
    )
{
    if (m_Connected)
    {
        if (m_TimeSource != NULL)
        {
            IGNOREHR(m_TimeSource->RemoveSink(this));
        }

        m_Connected = FALSE;
    }

    ReleaseObject(m_Timeline);
    ReleaseObject(m_TimeSource);
}

__checkReturn HRESULT 
CAnimationClock::Initialize(
    __in CTimeSource* pTimeSource, 
    __in CAnimationTimeline* pTimeline
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pTimeSource);
    IFCPTR(pTimeline);

    m_Timeline = pTimeline;
    AddRefObject(m_Timeline);

    m_TimeSource = pTimeSource;
    AddRefObject(m_TimeSource);

    IFC(m_TimeSource->AddSink(this));

    m_Connected = TRUE;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CAnimationClock::AddChangeListener(
    const ClockChangedHandler& Handler, 
    __out events::signals::connection* pConnection
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_ValueChanged.connect(Handler);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CAnimationClock::AddCompletedListener(
    const ClockCompletedHandler& Handler, 
    __out events::signals::connection* pConnection
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_Completed.connect(Handler);

Cleanup:
    return hr;
}

void 
CAnimationClock::RaiseValueChanged(
    )
{
    m_ValueChanged(this);
}

void 
CAnimationClock::RaiseCompleted(    
    )
{
    m_Completed(this);
}

__checkReturn HRESULT 
CAnimationClock::OnTimeUpdate(
    const CTimeSpan& TimeDelta
    )
{
    HRESULT hr = S_OK;
    CDurationValue* pDuration = NULL;

    switch(m_ClockState)
    {
        case ClockState::Active:
            {
                m_RunningDuration += TimeDelta;

                IFC(m_Timeline->GetTypedEffectiveValue(&CAnimationTimeline::DurationProperty, &pDuration));

                if(pDuration != NULL && pDuration->HasTimeSpan())
                {
                    const CTimeSpan* pTimeSpan = pDuration->GetTimeSpan();

                    FLOAT Progress = (m_RunningDuration / *pTimeSpan);

                    //if(Repeat)
                    //{
                        //m_Progress = Progress - std::floorf(Progress);
                    //}
                    //else
                    //{
                        if(Progress >= 1.0f)
                        {
                            IFC(Complete());
                        }

                        m_Progress = std::min(Progress, 1.0f);
                    //}
                }

                RaiseValueChanged();

                break;
            }

        case ClockState::Filling:
            {
                break;
            }
            
        case ClockState::Stopped:
            {
                break;
            }
    }

Cleanup:
    ReleaseObject(pDuration);

    return hr;
}

__checkReturn HRESULT
CAnimationClock::Complete(
    )
{
    HRESULT hr = S_OK;

    m_ClockState = ClockState::Filling;

    if (m_Connected)
    {
        IFC(m_TimeSource->RemoveSink(this));

        m_Connected = FALSE;
    }

    RaiseCompleted();

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CAnimationClock::GetCurrentValue(
    __in CObjectWithType* pDefaultInitialValue, 
    __in CObjectWithType* pDefaultFinalValue,
    __deref_out CObjectWithType** ppObject
    )
{
    HRESULT hr = S_OK;

    IFC(m_Timeline->GetCurrentValue(this, pDefaultInitialValue, pDefaultFinalValue, ppObject));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CAnimationClock::GetCurrentProgress(
    __out FLOAT* pProgress
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProgress);

    *pProgress = m_Progress;

Cleanup:
    return hr;
}