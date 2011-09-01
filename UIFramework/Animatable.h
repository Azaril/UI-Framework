#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "TimeSource.h"

class CAnimationTimeline;
class CAnimationClock;
class CAnimationInfo;

EXPIMP_TEMPLATE template class UIFRAMEWORK_API std::vector< CAnimationInfo* >;

class UIFRAMEWORK_API CAnimatable
{
    public:
        virtual HRESULT BeginAnimation( 
            __in CProperty* pProperty,
            __in CAnimationTimeline* pTimeline 
            );

        virtual HRESULT ApplyAnimationClock(
            __in CProperty* pProperty, 
            __in CAnimationClock* pClock 
            );

    protected:
        CAnimatable(
            );

        virtual ~CAnimatable(
            );

        virtual __out CTimeSource* GetTimeSource(
            ) = 0;

        virtual __checkReturn HRESULT GetAnimationBaseValue(
            __in CProperty* pProperty,
            __deref_out CObjectWithType** ppValue 
            ) = 0;

        virtual __checkReturn HRESULT SetAnimationValue(
            __in CProperty* pProperty,
            __in CObjectWithType* pValue 
            ) = 0;

        void OnClockChanged(
            __in CAnimationInfo* pInfo 
            );

        std::vector< CAnimationInfo* > m_Animations;
};

class CAnimationInfo : public CRefCountedObject
{
    public:
        CAnimationInfo( 
            __in CProperty* pProperty, 
            __in CObjectWithType* pInitialValue, 
            __in CAnimationClock* pClock 
            );

        virtual ~CAnimationInfo(
            );

        __out CProperty* GetProperty(
            );

        void SetProperty( 
            __in CProperty* pProperty 
            );

        __out CAnimationClock* GetClock(
            );

        void SetClock( 
            __in CAnimationClock* pClock 
            );

        void SetConnection(
            events::signals::connection Connection
            );

        void Disconnect(
            );

        __out CObjectWithType* GetInitialValue(
            );

    protected:
        CProperty* m_Property;
        CObjectWithType* m_InitialValue;
        CAnimationClock* m_Clock;
        events::signals::connection m_ChangeConnection;
};