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

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            const SizeF& AvailableSize,
            SizeF& UsedSize 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        void OnThumbDragDelta(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs);

        HRESULT ComputeSliderLengths( SizeF AvailableSize, Orientation::Value Orient, FLOAT* pDecreaseButtonLength, FLOAT* pThumbLength, FLOAT* pIncreaseButtonLength );
        HRESULT ComputeScrollBarLengths( SizeF AvailableSize, FLOAT ViewportSize, Orientation::Value Orient, FLOAT* pDecreaseButtonLength, FLOAT* pThumbLength, FLOAT* pIncreaseButtonLength, bool* pHide );

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

        CLayeredValue m_Thumb;
        CLayeredValue m_IncreaseButton;
        CLayeredValue m_DecreaseButton;
        CLayeredValue m_ViewportSize;
        CLayeredValue m_Orientation;
        CLayeredValue m_DirectionReversed;
        CLayeredValue m_Minimum;
        CLayeredValue m_Maximum;
        CLayeredValue m_TrackValue;
        FLOAT m_ThumbCenterOffset;
        FLOAT m_Density;

        events::signals::connection m_ThumbDragDeltaConnection;
};

template< >
struct ObjectTypeTraits< CTrack >
{
    static const TypeIndex::Value Type = TypeIndex::Track;
};