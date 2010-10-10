#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "TimeSource.h"

class CAnimationTimeline;
class CAnimationClock;
class CAnimationInfo;

class UIFRAMEWORK_API CAnimatable
{
    public:
        virtual HRESULT BeginAnimation( CProperty* pProperty, CAnimationTimeline* pTimeline );

        virtual HRESULT ApplyAnimationClock( CProperty* pProperty, CAnimationClock* pClock );

    protected:
        CAnimatable();
        virtual ~CAnimatable();

        virtual CTimeSource* GetTimeSource() = 0;
        virtual HRESULT GetAnimationBaseValue( CProperty* pProperty, CObjectWithType** ppValue ) = 0;
        virtual HRESULT SetAnimationValue( CProperty* pProperty, CObjectWithType* pValue ) = 0;

        void OnClockChanged( CAnimationInfo* pInfo );

        std::vector< CAnimationInfo* > m_Animations;
};

class CAnimationInfo : public CRefCountedObject
{
    public:
        CAnimationInfo( CProperty* pProperty, CObjectWithType* pInitialValue, CAnimationClock* pClock );
        ~CAnimationInfo();

        CProperty* GetProperty();
        void SetProperty( CProperty* pProperty );

        CAnimationClock* GetClock();
        void SetClock( CAnimationClock* pClock );

        void SetConnection(events::signals::connection Connection);
        void Disconnect();

        CObjectWithType* GetInitialValue();

    protected:
        CProperty* m_Property;
        CObjectWithType* m_InitialValue;
        CAnimationClock* m_Clock;
        events::signals::connection m_ChangeConnection;
};