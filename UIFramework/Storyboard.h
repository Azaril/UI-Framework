#pragma once

#include "PropertyObject.h"
#include "Factory.h"
#include "Providers.h"
#include "AnimationTimelineCollection.h"
#include "Animatable.h"

class CUIElement;

struct IResolvedAnimationCallback
{
    virtual __checkReturn HRESULT AddAnimation(
        __in CAnimatable* pTarget,
        __in CProperty* pProperty,
        __in CAnimationTimeline* pTimeline
        ) = 0;
};

class CStoryboard : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY1( CStoryboard, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Storyboard, CRefCountedObjectBase< CPropertyObject > );

        static __checkReturn HRESULT CreatePropertyInformation( 
			__deref_out CPropertyInformation** ppInformation 
			);

        __checkReturn HRESULT ResolveAnimations(
            __in CUIElement* pElement,
            __in IResolvedAnimationCallback* pCallback
            );

        //
        // Properties
        //
        static CStaticProperty ChildrenProperty;
        static CStaticProperty TargetProperty;
        static CStaticProperty TargetNameProperty;
        static CStaticProperty TargetPropertyProperty;

    protected:
        CStoryboard(
            );

        virtual ~CStoryboard(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        CLayeredValue m_Children;
        CLayeredValue m_Target;
        CLayeredValue m_TargetName;
        CLayeredValue m_TargetProperty;
};

template< >
struct ObjectTypeTraits< CStoryboard >
{
    static const TypeIndex::Value Type = TypeIndex::Storyboard;
};