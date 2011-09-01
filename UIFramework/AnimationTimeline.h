#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "TimeSource.h"
#include "AnimationClock.h"
#include "BasicTypes.h"

class CAnimationTimeline;

typedef events::signal< void ( CAnimationTimeline*, CAnimationClock* ) > TimelineCompletedSignal;
typedef TimelineCompletedSignal::slot_type TimelineCompletedHandler;

class UIFRAMEWORK_API CAnimationTimeline : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        virtual TypeIndex::Value GetProvidedType(
            ) = 0;

        __checkReturn __checkReturn HRESULT AddCompletedListener( 
            const TimelineCompletedHandler& Handler, 
            __out events::signals::connection* pConnection 
            );

        virtual __checkReturn HRESULT GetDuration( 
            __deref_out CDurationValue** ppDuration 
            );

        virtual __checkReturn HRESULT CreateClock( 
            __in CTimeSource* pTimeSource,
            __deref_out CAnimationClock** ppClock 
            );

        virtual __checkReturn HRESULT GetCurrentValue( 
            __in CAnimationClock* pClock, 
            __in CObjectWithType* pDefaultInitialValue, 
            __in CObjectWithType* pDefaultFinalValue, 
            __deref_out CObjectWithType** ppValue 
            ) = 0;

    protected:
        CAnimationTimeline(
            );

        virtual ~CAnimationTimeline(
            );

        void OnClockCompleted( 
            __in CAnimationClock* pClock 
            );

        TimelineCompletedSignal m_Completed;
        CDurationValue* m_Duration;
};