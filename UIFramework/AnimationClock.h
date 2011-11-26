#pragma once

#include "Types.h"
#include "Signals.h"
#include "RefCounted.h"
#include "TimeSink.h"
#include "Factory.h"
#include "PropertyObject.h"
#include "TimeSource.h"

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime

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

        virtual __checkReturn HRESULT GetCurrentValue( 
            __in CObjectWithType* pDefaultInitialValue, 
            __in CObjectWithType* pDefaultFinalValue,
            __deref_out CObjectWithType** ppObject 
            );

        __checkReturn HRESULT AddChangeListener( 
            const ClockChangedHandler& Handler, 
            __out events::signals::connection* pConnection 
            );

        __checkReturn HRESULT AddCompletedListener( 
            const ClockCompletedHandler& Handler, 
            __out events::signals::connection* pConnection
            );

        __override virtual __checkReturn HRESULT OnTimeUpdate( 
            const CTimeSpan& TimeDelta 
            );

        __checkReturn HRESULT GetCurrentTime(
            __out CTimeSpan* pTime 
            );

        __checkReturn HRESULT GetCurrentProgress( 
            __out FLOAT* pProgress 
            );

        __checkReturn HRESULT AddAnimationTarget(
            __in CPropertyObject* pPropertyObject,
            __in CProperty* pProperty
            );

        __checkReturn HRESULT RemoveAnimationTarget(
            __in CPropertyObject* pPropertyObject,
            __in CProperty* pProperty
            );

    protected:
        CAnimationClock(
            );

        virtual ~CAnimationClock(
            );

        __checkReturn HRESULT Initialize(
            __in CTimeSource* pTimeSource,
            __in CAnimationTimeline* pTimeline 
            );

        __checkReturn HRESULT Complete(
            );

        void RaiseValueChanged(
            );

        void RaiseCompleted(
            );

        CTimeSource* m_TimeSource;
        CAnimationTimeline* m_Timeline;
        CTimeSpan m_RunningDuration;
        ClockChangedSignal m_ValueChanged;
        ClockCompletedSignal m_Completed;
        bool m_Connected;
        ClockState::Value m_ClockState;
        FLOAT m_Progress;
};

#pragma pop_macro("GetCurrentTime")