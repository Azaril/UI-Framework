#pragma once

#include "Control.h"
#include "StaticRoutedCommand.h"

class CScrollBar : public CControl
{
    public:
        DECLARE_FACTORY1( CScrollBar, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ScrollBar, CControl );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );
        static HRESULT CreateCommandInformation( CCommandInformation** ppInformation );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Properties
        //
        static CStaticProperty OrientationProperty;

        //
        // Commands
        //
        static CStaticRoutedCommand LineUpCommand;
        static CStaticRoutedCommand LineDownCommand;

    protected:
        CScrollBar();
        virtual ~CScrollBar();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT GetEffectiveOrientation( Orientation::Value* pOrientation );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnOrientationChanged );

        HRESULT OnOrientationChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CTypedLayeredValue< COrientationValue > m_Orientation;
};

template< >
struct ObjectTypeTraits< CScrollBar >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollBar;
};