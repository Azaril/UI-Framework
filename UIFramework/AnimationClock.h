#pragma once

#include "Types.h"
#include "Signals.h"
#include "RefCounted.h"
#include "TimeSink.h"
#include "Factory.h"
#include "PropertyObject.h"
#include "TimeSource.h"

class CAnimationClock;
class CAnimationTimeline;

typedef events::signal< void ( CAnimationClock* ) > ClockChangedSignal;
typedef ClockChangedSignal::slot_type ClockChangedHandler;

typedef events::signal< void ( CAnimationClock* ) > ClockCompletedSignal;;
typedef ClockCompletedSignal::slot_type ClockCompletedHandler;

namespace ClockState
{
    enum Value
    {
        Active,
        Filling,
        Stopped
    };
}

class UIFRAMEWORK_API CAnimationClock : public CRefCountedObjectBase< CTimeSink >
{
    public:
        DECLARE_FACTORY2( CAnimationClock, CTimeSource*, CAnimationTimeline* );

        virtual HRESULT GetCurrentValue( CObjectWithType* pDefaultInitialValue, CObjectWithType* pDefaultFinalValue, CObjectWithType** ppObject );

        HRESULT AddChangeListener( const ClockChangedHandler& Handler, events::signals::connection* pConnection );
        HRESULT AddCompletedListener( const ClockCompletedHandler& Handler, events::signals::connection* pConnection );

        virtual HRESULT OnTimeUpdate( const CTimeSpan& TimeDelta );

        HRESULT GetCurrentTime( CTimeSpan* pTime );
        HRESULT GetCurrentProgress( FLOAT* pProgress );

    protected:
        CAnimationClock();
        virtual ~CAnimationClock();

        HRESULT Initialize( CTimeSource* pTimeSource, CAnimationTimeline* pTimeline );

        void RaiseValueChanged();
        void RaiseCompleted();

        CTimeSource* m_TimeSource;
        CAnimationTimeline* m_Timeline;
        CTimeSpan m_RunningDuration;
        ClockChangedSignal m_ValueChanged;
        ClockCompletedSignal m_Completed;
        BOOL m_Connected;
        ClockState::Value m_ClockState;
        FLOAT m_Progress;
};
