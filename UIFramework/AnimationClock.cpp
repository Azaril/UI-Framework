#include "AnimationClock.h"
#include "AnimationTimeline.h"
#include "BasicTypes.h"

CAnimationClock::CAnimationClock() : m_RunningDuration(CTimeSpan::Zero()),
                                     m_Timeline(NULL),
                                     m_TimeSource(NULL),
                                     m_Connected(FALSE),
                                     m_ClockState(ClockState::Active),
                                     m_Progress(0)
{
}

CAnimationClock::~CAnimationClock()
{
    ReleaseObject(m_Timeline);
}

HRESULT CAnimationClock::Initialize(CTimeSource* pTimeSource, CAnimationTimeline* pTimeline)
{
    HRESULT hr = S_OK;

    IFCPTR(pTimeSource);
    IFCPTR(pTimeline);

    m_Timeline = pTimeline;
    AddRefObject(m_Timeline);

    m_TimeSource = pTimeSource;
    AddRefObject(m_TimeSource);

    IFC(m_TimeSource->AddSink(this));

Cleanup:
    return hr;
}

HRESULT CAnimationClock::AddChangeListener(const ClockChangedHandler& Handler, events::signals::connection* pConnection)
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    if(!m_Connected)
    {
        IFC(m_TimeSource->AddSink(this));

        m_Connected = TRUE;
    }

    *pConnection = m_ValueChanged.connect(Handler);

Cleanup:
    return hr;
}

HRESULT CAnimationClock::AddCompletedListener(const ClockCompletedHandler& Handler, events::signals::connection* pConnection)
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_Completed.connect(Handler);

Cleanup:
    return hr;
}

void CAnimationClock::RaiseValueChanged()
{
    m_ValueChanged(this);
}

void CAnimationClock::RaiseCompleted()
{
    m_Completed(this);
}

HRESULT CAnimationClock::OnTimeUpdate(const CTimeSpan& TimeDelta)
{
    HRESULT hr = S_OK;
    CDurationValue* pDuration = NULL;

    switch(m_ClockState)
    {
        case ClockState::Active:
            {
                m_RunningDuration += TimeDelta;

                IFC(m_Timeline->GetDuration(&pDuration));

                if(pDuration && pDuration->HasTimeSpan())
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
                            m_ClockState = ClockState::Filling;

                            RaiseCompleted();
                        }

                        m_Progress = std::min(Progress, 1.0f);
                    //}
                }

                RaiseValueChanged();

                break;
            }
    }

    //if(m_Connected.empty())
    //{
    //    IFC(m_TimeSource->RemoveSink(this));

    //    m_Connected = FALSE;

    //    goto Cleanup;
    //}    

Cleanup:
    ReleaseObject(pDuration);

    return hr;
}

HRESULT CAnimationClock::GetCurrentValue(CObjectWithType* pDefaultInitialValue, CObjectWithType* pDefaultFinalValue, CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;

    IFC(m_Timeline->GetCurrentValue(this, pDefaultInitialValue, pDefaultFinalValue, ppObject));

Cleanup:
    return hr;
}

HRESULT CAnimationClock::GetCurrentProgress(FLOAT* pProgress)
{
    HRESULT hr = S_OK;
    CDurationValue* pDuration = NULL;

    IFCPTR(pProgress);

    *pProgress = m_Progress;

Cleanup:
    return hr;
}