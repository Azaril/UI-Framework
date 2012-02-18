#pragma once

#include "TriggerAction.h"
#include "Providers.h"
#include "StaticPropertyInformation.h"
#include "Storyboard.h"

class CBeginStoryboard : public CRefCountedObjectBase< CTriggerAction >
{
    public:
        DECLARE_FACTORY1( CBeginStoryboard, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::BeginStoryboard, CTriggerAction );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation
            );

        virtual __checkReturn HRESULT ResolveAction(
            __in CUIElement* pObject,
            __in IStyleCallback* pCallback,
            __deref_out CResolvedTriggerAction** ppResolvedSetter 
            );

        //
        // Properties
        //
        static CStaticProperty StoryboardProperty;

    protected:
        CBeginStoryboard(
            );

        virtual ~CBeginStoryboard(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        CLayeredValue m_Storyboard;
};

template< >
struct ObjectTypeTraits< CBeginStoryboard >
{
    static const TypeIndex::Value Type = TypeIndex::BeginStoryboard;
};

class CTimeController;

class CResolvedBeginStoryboard : public CResolvedTriggerAction,
                                 public IResolvedAnimationCallback,
                                 private CTimeSink
{
    public:
        DELEGATE_REFCOUNTING( CResolvedTriggerAction );

        DECLARE_FACTORY1( CResolvedBeginStoryboard, CTimeSource* );

        __override virtual __checkReturn HRESULT AddAnimation(
            __in CAnimatable* pTarget,
            __in CProperty* pProperty,
            __in CAnimationTimeline* pTimeline
            );

        __override virtual __checkReturn HRESULT Apply(
            );

    protected:
        CResolvedBeginStoryboard(
            );

        virtual ~CResolvedBeginStoryboard(
            );

        __checkReturn HRESULT Initialize(
            __in CTimeSource* pTimeSource
            );

        __override virtual __checkReturn HRESULT OnTimeUpdate( 
            const CTimeSpan& TimeDelta 
            );

        class CClockInfo
        {
            public:
                CClockInfo(
                    __in CAnimatable* pAnimatable,
                    __in CProperty* pProperty,
                    __in CAnimationTimeline* pTimeline,
                    __in_opt CAnimationClock* pClock
                    )
                    : m_pAnimatable(pAnimatable)
                    , m_pProperty(pProperty)
                    , m_pTimeline(pTimeline)
                    , m_pClock(pClock)
                {
                    AddRefObject(m_pAnimatable);
                    AddRefObject(m_pProperty);
                    AddRefObject(m_pTimeline);
                    AddRefObject(m_pClock);
                }

                CClockInfo(
                    const CClockInfo& Other
                    )
                    : m_pAnimatable(Other.m_pAnimatable)
                    , m_pProperty(Other.m_pProperty)
                    , m_pTimeline(Other.m_pTimeline)
                    , m_pClock(Other.m_pClock)                    
                {
                    AddRefObject(m_pAnimatable);
                    AddRefObject(m_pProperty);
                    AddRefObject(m_pTimeline);
                    AddRefObject(m_pClock);
                }


                ~CClockInfo(
                    )
                {
                    ReleaseObject(m_pAnimatable);
                    ReleaseObject(m_pProperty);
                    ReleaseObject(m_pTimeline);
                    ReleaseObject(m_pClock);                    
                }

                CClockInfo& operator=(
                    const CClockInfo& Other
                    )
                {
                    ReplaceObject(m_pAnimatable, Other.GetAnimatable());
                    ReplaceObject(m_pProperty, Other.GetProperty());
                    ReplaceObject(m_pTimeline, Other.GetTimeline());
                    ReplaceObject(m_pClock, Other.GetClock());

                    return (*this);
                }

                __out CAnimatable* GetAnimatable(
                    ) const
                {
                    return m_pAnimatable;
                }

                void SetClock(
                    __in_opt CAnimationClock* pClock
                    )
                {
                    ReplaceObject(m_pClock, pClock);
                }

                __out CAnimationClock* GetClock(
                    ) const
                {
                    return m_pClock;
                }

                __out CAnimationTimeline* GetTimeline(
                    ) const
                {
                    return m_pTimeline;
                }

                __out CProperty* GetProperty(
                    ) const
                {
                    return m_pProperty;
                }

            protected:
                CAnimatable* m_pAnimatable;
                CAnimationClock* m_pClock;
                CAnimationTimeline* m_pTimeline;
                CProperty* m_pProperty;
        };

        CTimeSource* m_pTimeSource;
        bool m_RegisteredToTimeSource;
        CTimeController* m_pTimeController;
        vector< CClockInfo > m_Animations;
};