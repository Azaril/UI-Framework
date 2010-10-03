#pragma once

#include "Control.h"
#include "Thumb.h"
#include "Button.h"

class CTrack : public CControl
{
    public:
        DECLARE_FACTORY1( CTrack, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Track, CControl );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Properties
        //
        static CStaticProperty ThumbProperty;
        static CStaticProperty IncreaseButtonProperty;
        static CStaticProperty DecreaseButtonProperty;
        static CStaticProperty ViewportSizeProperty;
        static CStaticProperty OrientationProperty;
        static CStaticProperty DirectionReversedProperty;
        static CStaticProperty MinimumProperty;
        static CStaticProperty MaximumProperty;
        static CStaticProperty ValueProperty;

    protected:
        CTrack();
        virtual ~CTrack();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        void OnThumbDragDelta(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs);

        HRESULT ComputeSliderLengths( SizeF AvailableSize, Orientation::Value Orient, FLOAT* pDecreaseButtonLength, FLOAT* pThumbLength, FLOAT* pIncreaseButtonLength );
        HRESULT ComputeScrollBarLengths( SizeF AvailableSize, FLOAT ViewportSize, Orientation::Value Orient, FLOAT* pDecreaseButtonLength, FLOAT* pThumbLength, FLOAT* pIncreaseButtonLength, BOOL* pHide );

        HRESULT GetEffectiveThumb( CThumb** ppThumb );
        HRESULT GetEffectiveIncreaseButton( CButton** ppIncreaseButton );
        HRESULT GetEffectiveDecreaseButton( CButton** ppDecreaseButton );
        HRESULT GetEffectiveViewportSize( FLOAT* pViewportSize );
        HRESULT GetEffectiveOrientation( Orientation::Value* pOrientation );
        HRESULT GetEffectiveDirectionReversed( BOOL* pReversed );
        HRESULT GetEffectiveMinimum( FLOAT* pMinimum );
        HRESULT GetEffectiveMaximum( FLOAT* pMaximum );
        HRESULT GetEffectiveTrackValue( FLOAT* pValue );

        HRESULT SetTrackValue( FLOAT Value );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnThumbChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnIncreaseButtonChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnDecreaseButtonChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnViewportSizeChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnOrientationChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnDirectionReversedChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMinimumChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnMaximumChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnTrackValueChanged );

        HRESULT OnThumbChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnIncreaseButtonChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnDecreaseButtonChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnViewportSizeChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnOrientationChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnDirectionReversedChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMinimumChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnMaximumChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnTrackValueChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CTypedLayeredValue< CTrack, CThumb > m_Thumb;
        CTypedLayeredValue< CTrack, CButton > m_IncreaseButton;
        CTypedLayeredValue< CTrack, CButton > m_DecreaseButton;
        CTypedLayeredValue< CTrack, CFloatValue > m_ViewportSize;
        CTypedLayeredValue< CTrack, COrientationValue > m_Orientation;
        CTypedLayeredValue< CTrack, CBoolValue > m_DirectionReversed;
        CTypedLayeredValue< CTrack, CFloatValue > m_Minimum;
        CTypedLayeredValue< CTrack, CFloatValue > m_Maximum;
        CTypedLayeredValue< CTrack, CFloatValue > m_TrackValue;
        FLOAT m_ThumbCenterOffset;
        FLOAT m_Density;

        events::signals::connection m_ThumbDragDeltaConnection;
};

template< >
struct ObjectTypeTraits< CTrack >
{
    static const TypeIndex::Value Type = TypeIndex::Track;
};