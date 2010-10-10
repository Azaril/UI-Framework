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
        virtual TypeIndex::Value GetProvidedType() = 0;

        HRESULT AddCompletedListener( const TimelineCompletedHandler& Handler, events::signals::connection* pConnection );

        virtual HRESULT GetDuration( CDurationValue** ppDuration );

        virtual HRESULT CreateClock( CTimeSource* pTimeSource, CAnimationClock** ppClock );

        virtual HRESULT GetCurrentValue( CAnimationClock* pClock, CObjectWithType* pDefaultInitialValue, CObjectWithType* pDefaultFinalValue, CObjectWithType** ppValue ) = 0;

    protected:
        CAnimationTimeline();
        virtual ~CAnimationTimeline();

        void OnClockCompleted( CAnimationClock* pClock );

        TimelineCompletedSignal m_Completed;
        CDurationValue* m_Duration;
};