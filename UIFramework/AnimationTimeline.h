#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "TimeSource.h"
#include "AnimationClock.h"
#include "BasicTypes.h"
#include "StaticPropertyInformation.h"

class CAnimationTimeline;

typedef events::signal< void ( CAnimationTimeline*, CAnimationClock* ) > TimelineCompletedSignal;
typedef TimelineCompletedSignal::slot_type TimelineCompletedHandler;

class UIFRAMEWORK_API CAnimationTimeline : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::AnimationTimeline, CRefCountedObjectBase< CPropertyObject > );

        static __checkReturn HRESULT CreatePropertyInformation( 
			__deref_out CPropertyInformation** ppInformation 
			);

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

        //
        // Properties
        //
        static CStaticProperty DurationProperty;

    protected:
        CAnimationTimeline(
            );

        virtual ~CAnimationTimeline(
            );

        __override virtual __checkReturn HRESULT SetValueInternal(
            __in CProperty* pProperty,
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetValueInternal(
            __in CProperty* pProperty, 
            __deref_out_opt CObjectWithType** ppValue 
            );

        void OnClockCompleted( 
            __in CAnimationClock* pClock 
            );

        TimelineCompletedSignal m_Completed;
        CDurationValue* m_Duration;
};

template< >
struct ObjectTypeTraits< CAnimationTimeline >
{
    static const TypeIndex::Value Type = TypeIndex::AnimationTimeline;
};