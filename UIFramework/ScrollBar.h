#pragma once

#include "RangeBase.h"
#include "StaticRoutedCommand.h"
#include "Track.h"

class CScrollBar : public CRangeBase
{
    public:
        DECLARE_FACTORY1( CScrollBar, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ScrollBar, CRangeBase );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );
        static HRESULT CreateCommandInformation( CCommandInformation** ppInformation );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Properties
        //
        static CStaticProperty OrientationProperty;
        static CStaticProperty ViewportSizeProperty;

        //
        // Commands
        //
        static CStaticRoutedCommand LineUpCommand;
        static CStaticRoutedCommand LineDownCommand;

    protected:
        CScrollBar();
        virtual ~CScrollBar();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT PostTemplateApplied();
        virtual HRESULT PreTemplateRevoked();

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT GetEffectiveOrientation( Orientation::Value* pOrientation );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnOrientationChanged );

        HRESULT OnOrientationChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CTypedLayeredValue< CScrollBar, COrientationValue > m_Orientation;
        CTypedLayeredValue< CScrollBar, CFloatValue > m_ViewportSize;
        CTrack* m_Track;
};

template< >
struct ObjectTypeTraits< CScrollBar >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollBar;
};