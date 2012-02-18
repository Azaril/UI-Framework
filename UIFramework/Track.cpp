#include "Track.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"
#include "DragDeltaEventArgs.h"
#include "SystemValues.h"
#include <math.h>

//
// Properties
//
namespace TrackProperties
{
    enum Value
    {
        Thumb,
        IncreaseButton,
        DecreaseButton,
        ViewportSize,
        Orientation,
        DirectionReversed,
        Minimum,
        Maximum,
        Value
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Thumb );
DEFINE_GET_DEFAULT_NULL( IncreaseButton );
DEFINE_GET_DEFAULT_NULL( DecreaseButton );
DEFINE_GET_DEFAULT( ViewportSize, FLOAT, std::numeric_limits< FLOAT >::quiet_NaN() );
DEFINE_GET_DEFAULT( Orientation, Orientation::Value, Orientation::Vertical );
DEFINE_GET_DEFAULT( DirectionReversed, bool, FALSE );
DEFINE_GET_DEFAULT( Minimum, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( Maximum, FLOAT, 1.0f );
DEFINE_GET_DEFAULT( TrackValue, FLOAT, 0.0f );

//
// Properties
//
CStaticProperty CTrack::ThumbProperty(TypeIndex::Track, TrackProperties::Thumb, L"Thumb", TypeIndex::Thumb, StaticPropertyFlags::None, &GET_DEFAULT( Thumb ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnThumbChanged ));
CStaticProperty CTrack::IncreaseButtonProperty(TypeIndex::Track, TrackProperties::IncreaseButton, L"IncreaseButton", TypeIndex::Button, StaticPropertyFlags::None, &GET_DEFAULT( IncreaseButton ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnIncreaseButtonChanged ));
CStaticProperty CTrack::DecreaseButtonProperty(TypeIndex::Track, TrackProperties::DecreaseButton, L"DecreaseButton", TypeIndex::Button, StaticPropertyFlags::None, &GET_DEFAULT( DecreaseButton ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnDecreaseButtonChanged ));
CStaticProperty CTrack::ViewportSizeProperty(TypeIndex::Track, TrackProperties::ViewportSize, L"ViewportSize", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( ViewportSize ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnViewportSizeChanged ));
CStaticProperty CTrack::OrientationProperty(TypeIndex::Track, TrackProperties::Orientation, L"Orientation", TypeIndex::Orientation, StaticPropertyFlags::None, &GET_DEFAULT( Orientation ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnOrientationChanged ));
CStaticProperty CTrack::DirectionReversedProperty(TypeIndex::Track, TrackProperties::DirectionReversed, L"DirectionReversed", TypeIndex::Bool, StaticPropertyFlags::None, &GET_DEFAULT( DirectionReversed ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnDirectionReversedChanged ));
CStaticProperty CTrack::MinimumProperty(TypeIndex::Track, TrackProperties::Minimum, L"Minimum", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Minimum ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnMinimumChanged ));
CStaticProperty CTrack::MaximumProperty(TypeIndex::Track, TrackProperties::Maximum, L"Maximum", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Maximum ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnMaximumChanged ));
CStaticProperty CTrack::ValueProperty(TypeIndex::Track, TrackProperties::Value, L"Value", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( TrackValue ), &INSTANCE_CHANGE_CALLBACK( CTrack, OnTrackValueChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnThumbChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnIncreaseButtonChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnDecreaseButtonChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnViewportSizeChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnOrientationChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnDirectionReversedChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnMinimumChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnMaximumChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CTrack, OnTrackValueChanged );

CTrack::CTrack(
    ) 
    : m_ThumbCenterOffset(std::numeric_limits< FLOAT >::quiet_NaN())
    , m_Density(std::numeric_limits< FLOAT >::quiet_NaN())                   
{
}

CTrack::~CTrack()
{
    m_ThumbDragDeltaConnection.disconnect();
}

HRESULT CTrack::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CControl::Initialize(pProviders));

    IFC(AddHandler(&CThumb::DragDeltaEvent, boost::bind(&CTrack::OnThumbDragDelta, this, _1, _2), &m_ThumbDragDeltaConnection));

Cleanup:
    return hr;
}

void CTrack::OnThumbDragDelta(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CDragDeltaEventArgs* pDragEventArgs = NULL;
    Orientation::Value Orient;
    bool DirectionReversed = FALSE;
    FLOAT TrackValue = 0.0f;
    
    IFC(CastType(pRoutedEventArgs, &pDragEventArgs));

    IFC(GetBasicTypeEffectiveValue(&OrientationProperty, &Orient));
    IFC(GetBasicTypeEffectiveValue(&DirectionReversedProperty, &DirectionReversed));
    IFC(GetBasicTypeEffectiveValue(&ValueProperty, &TrackValue));

    if(Orient == Orientation::Vertical)
    {
        FLOAT Delta = pDragEventArgs->GetVerticalDelta() * m_Density * (DirectionReversed ? 1.0f : -1.0f);

        IFC(SetTrackValue(TrackValue + Delta));
    }
    else
    {
        FLOAT Delta = pDragEventArgs->GetHorizontalDelta() * m_Density * (DirectionReversed ? -1.0f : 1.0f);

        IFC(SetTrackValue(TrackValue + Delta));
    }

    pRoutedEventArgs->SetHandled();

Cleanup:
    ;
}

HRESULT CTrack::SetTrackValue(FLOAT Value)
{
    HRESULT hr = S_OK;
    FLOAT Minimum = 0;
    FLOAT Maximum = 0;
    CFloatValue* pValue = NULL;

    IFC(GetBasicTypeEffectiveValue(&MaximumProperty, &Maximum));
    IFC(GetBasicTypeEffectiveValue(&MinimumProperty, &Minimum));

    Value = std::min(std::max(Value, Minimum), Maximum);

    IFC(CFloatValue::Create(Value, &pValue));

    IFC(SetValue(&ValueProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

HRESULT CTrack::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ThumbProperty,
        &IncreaseButtonProperty,
        &DecreaseButtonProperty,
        &ViewportSizeProperty,
        &OrientationProperty,
        &DirectionReversedProperty,
        &MinimumProperty,
        &MaximumProperty,
        &ValueProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CControl::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CTrack::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CTrack::ThumbProperty)
    {
        *ppLayeredValue = &m_Thumb;
    }
    else if(pProperty == &CTrack::IncreaseButtonProperty)
    {
        *ppLayeredValue = &m_IncreaseButton;
    }
    else if(pProperty == &CTrack::DecreaseButtonProperty)
    {
        *ppLayeredValue = &m_DecreaseButton;
    }
    else if(pProperty == &CTrack::ViewportSizeProperty)
    {
        *ppLayeredValue = &m_ViewportSize;
    }
    else if(pProperty == &CTrack::OrientationProperty)
    {
        *ppLayeredValue = &m_Orientation;
    }
    else if(pProperty == &CTrack::DirectionReversedProperty)
    {
        *ppLayeredValue = &m_DirectionReversed;
    }
    else if(pProperty == &CTrack::MinimumProperty)
    {
        *ppLayeredValue = &m_Minimum;
    }
    else if(pProperty == &CTrack::MaximumProperty)
    {
        *ppLayeredValue = &m_Maximum;
    }
    else if(pProperty == &CTrack::ValueProperty)
    {
        *ppLayeredValue = &m_TrackValue;
    }
    else
    {
        hr = CControl::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CTrack::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    CHitTestResult* pVisualHitTestResult = NULL;

    IFCPTR(ppHitTestResult);

    //IFC(m_BorderVisual->HitTest(LocalPoint, &pVisualHitTestResult));

    if(pVisualHitTestResult)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

    *ppHitTestResult = pVisualHitTestResult;

Cleanup:
    ReleaseObject(pVisualHitTestResult);

    return hr;
}

HRESULT CTrack::OnThumbChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CThumb* pOldThumb = NULL;
    CThumb* pNewThumb = NULL;

    if(pOldValue != NULL)
    {
        IFC(CastType(pOldValue, &pOldThumb));

        IFC(RemoveLogicalChild(pOldThumb));
    }

    if(pNewValue != NULL)
    {
        IFC(CastType(pNewValue, &pNewThumb));

        IFC(AddLogicalChild(pNewThumb));
    }

    InvalidateMeasure();
    InvalidateArrange();

Cleanup:
    return hr;
}

HRESULT CTrack::OnIncreaseButtonChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CButton* pOldButton = NULL;
    CButton* pNewButton = NULL;

    if(pOldValue != NULL)
    {
        IFC(CastType(pOldValue, &pOldButton));

        IFC(RemoveLogicalChild(pOldButton));
    }

    if(pNewValue != NULL)
    {
        IFC(CastType(pNewValue, &pNewButton));

        IFC(AddLogicalChild(pNewButton));
    }

    InvalidateMeasure();
    InvalidateArrange();

Cleanup:
    return hr;
}

HRESULT CTrack::OnDecreaseButtonChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CButton* pOldButton = NULL;
    CButton* pNewButton = NULL;

    if(pOldValue != NULL)
    {
        IFC(CastType(pOldValue, &pOldButton));

        IFC(RemoveLogicalChild(pOldButton));
    }

    if(pNewValue != NULL)
    {
        IFC(CastType(pNewValue, &pNewButton));

        IFC(AddLogicalChild(pNewButton));
    }

    InvalidateMeasure();
    InvalidateArrange();

Cleanup:
    return hr;
}

HRESULT CTrack::OnViewportSizeChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    InvalidateMeasure();
    InvalidateArrange();

    return hr;
}

HRESULT CTrack::OnOrientationChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    InvalidateMeasure();
    InvalidateArrange();

    return hr;
}

HRESULT CTrack::OnDirectionReversedChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    InvalidateMeasure();
    InvalidateArrange();

    return hr;
}

HRESULT CTrack::OnMinimumChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    InvalidateMeasure();
    InvalidateArrange();

    return hr;
}

HRESULT CTrack::OnMaximumChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    InvalidateMeasure();
    InvalidateArrange();

    return hr;
}

HRESULT CTrack::OnTrackValueChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    InvalidateMeasure();
    InvalidateArrange();

    return hr;
}

__override __checkReturn HRESULT 
CTrack::MeasureInternal( 
    const SizeF& AvailableSize,
    SizeF& DesiredSize
    )
{
    HRESULT hr = S_OK;   
    CThumb* pThumb = NULL;
    CButton* pIncreaseButton = NULL;
    CButton* pDecreaseButton = NULL;
    SizeF NeededSize;
    Orientation::Value TrackOrientation;
    FLOAT ViewportSize = 0;

    IFC(GetTypedEffectiveValue(&ThumbProperty, &pThumb));

    IFC(GetTypedEffectiveValue(&IncreaseButtonProperty, &pIncreaseButton));

    IFC(GetTypedEffectiveValue(&DecreaseButtonProperty, &pDecreaseButton));

    IFC(GetBasicTypeEffectiveValue(&ViewportSizeProperty, &ViewportSize));

    IFC(GetBasicTypeEffectiveValue(&OrientationProperty, &TrackOrientation));

    if (pThumb != NULL)
    {
        IFC(pThumb->Measure(AvailableSize));

        NeededSize = pThumb->GetDesiredSize();
    }

    if(!_isnan(ViewportSize))
    {
        if (TrackOrientation == Orientation::Vertical)
        {
            NeededSize.height = 0.0;
        }
        else
        {
            NeededSize.width = 0.0;
        }
    }

    DesiredSize = NeededSize;

Cleanup:
    ReleaseObject(pThumb);
    ReleaseObject(pIncreaseButton);
    ReleaseObject(pDecreaseButton);

    return hr;
}

inline void CoerceLength(FLOAT& componentLength, FLOAT trackLength)
{
    if (componentLength < 0)
    {
        componentLength = 0.0;
    }
    else if (componentLength > trackLength || _isnan(componentLength))
    {
        componentLength = trackLength;
    }
}

HRESULT CTrack::ComputeSliderLengths(SizeF AvailableSize, Orientation::Value Orient, FLOAT* pDecreaseButtonLength, FLOAT* pThumbLength, FLOAT* pIncreaseButtonLength)
{
    HRESULT hr = S_OK;
    FLOAT Minimum = 0;
    FLOAT Maximum = 0;
    FLOAT Value = 0;
    CThumb* pThumb = NULL;

    IFC(GetBasicTypeEffectiveValue(&MinimumProperty, &Minimum));
    IFC(GetBasicTypeEffectiveValue(&MaximumProperty, &Maximum));
    IFC(GetBasicTypeEffectiveValue(&ValueProperty, &Value));

    IFC(GetTypedEffectiveValue(&ThumbProperty, &pThumb));

    {
        FLOAT Range = std::max(0.0f, Maximum - Minimum);
        FLOAT Offset = std::min(Range, Value - Minimum);

        FLOAT TrackLength = 0;
        FLOAT ThumbLength = 0;

        if (Orient == Orientation::Vertical)
        {
            TrackLength = AvailableSize.height;
            ThumbLength = (pThumb == NULL) ? 0.0f : pThumb->GetDesiredSize().height;
        }
        else
        {
            TrackLength = AvailableSize.width;
            ThumbLength = (pThumb == NULL) ? 0.0f : pThumb->GetDesiredSize().width;
        }

        CoerceLength(ThumbLength, TrackLength);

        FLOAT RemainingTrackLength = TrackLength - ThumbLength;

        FLOAT DecreaseButtonLength = RemainingTrackLength * Offset / Range;
        CoerceLength(DecreaseButtonLength, RemainingTrackLength);
               
        FLOAT IncreaseButtonLength = RemainingTrackLength - DecreaseButtonLength;
        CoerceLength(IncreaseButtonLength, RemainingTrackLength);

        IFCEXPECT(DecreaseButtonLength >= 0.0f && DecreaseButtonLength <= RemainingTrackLength);
        IFCEXPECT(IncreaseButtonLength >= 0.0f && IncreaseButtonLength <= RemainingTrackLength);
                
        m_Density = Range / RemainingTrackLength;

        *pDecreaseButtonLength = DecreaseButtonLength;
        *pThumbLength = ThumbLength;
        *pIncreaseButtonLength = IncreaseButtonLength;
    }

Cleanup:
    ReleaseObject(pThumb);

    return hr;
}

HRESULT CTrack::ComputeScrollBarLengths(SizeF AvailableSize, FLOAT ViewportSize, Orientation::Value Orient, FLOAT* pDecreaseButtonLength, FLOAT* pThumbLength, FLOAT* pIncreaseButtonLength, bool* pHide)
{
    HRESULT hr = S_OK;
    FLOAT Minimum = 0;
    FLOAT Maximum = 0;
    FLOAT Value = 0;
    CThumb* pThumb = NULL;
    Visibility::Value Vis = Visibility::Visible;

    IFC(GetBasicTypeEffectiveValue(&MinimumProperty, &Minimum));
    IFC(GetBasicTypeEffectiveValue(&MaximumProperty, &Maximum));
    IFC(GetBasicTypeEffectiveValue(&ValueProperty, &Value));

    IFC(GetBasicTypeEffectiveValue(&VisibilityProperty, &Vis));

    IFC(GetTypedEffectiveValue(&ThumbProperty, &pThumb));

    {
        FLOAT Range = std::max(0.0f, Maximum - Minimum);
        FLOAT Offset = std::min(Range, Value - Minimum);

        IFCEXPECT(Offset >= 0);

        FLOAT Extent = std::max(0.0f, Range) + ViewportSize;

        FLOAT TrackLength = 0;

        FLOAT ThumbMinimumLength = 0;
        FLOAT DecreaseButtonLength = 0;
        FLOAT IncreaseButtonLength = 0;

        if (Orient == Orientation::Vertical)
        {
            TrackLength = AvailableSize.height;
            
            FLOAT ButtonHeight = VERTICAL_SCROLL_BUTTON_HEIGHT;

            ThumbMinimumLength = floorf(ButtonHeight * 0.5f);
        }
        else
        {
            TrackLength = AvailableSize.width;

            FLOAT ButtonWidth = HORIZONTAL_SCROLL_BUTTON_WIDTH;

            ThumbMinimumLength = floorf(ButtonWidth * 0.5f);
        }

        FLOAT ThumbLength =  TrackLength * ViewportSize / Extent;

        CoerceLength(ThumbLength, TrackLength);
             
        ThumbLength = std::max(ThumbMinimumLength, ThumbLength);

        bool NotEnoughContentToScroll = (Range < 0.0);
        bool ThumbLongerThanTrack = (ThumbLength > TrackLength);

        if (NotEnoughContentToScroll || ThumbLongerThanTrack)
        {
            if(Vis != Visibility::Hidden)
            {
                IFC(SetVisibility(Visibility::Hidden));
            }

            m_ThumbCenterOffset = std::numeric_limits< FLOAT >::quiet_NaN();
            m_Density = std::numeric_limits< FLOAT >::quiet_NaN();
            
            DecreaseButtonLength = 0.0;
            IncreaseButtonLength = 0.0;

            *pHide = TRUE;
        }
        else
        {
            *pHide = FALSE;

            if(Vis != Visibility::Visible)
            {
                IFC(SetVisibility(Visibility::Visible));
            }
        }

        {
            // Compute lengths of increase and decrease button
            FLOAT RemainingTrackLength = TrackLength - ThumbLength;
            DecreaseButtonLength = RemainingTrackLength * Offset / Range;
            CoerceLength(DecreaseButtonLength, RemainingTrackLength);

            IncreaseButtonLength = RemainingTrackLength - DecreaseButtonLength;
            CoerceLength(IncreaseButtonLength, RemainingTrackLength);

            m_Density = Range / RemainingTrackLength;

            *pDecreaseButtonLength = DecreaseButtonLength;
            *pIncreaseButtonLength = IncreaseButtonLength;
            *pThumbLength = ThumbLength;
        }
    }
    
Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CTrack::ArrangeInternal(
    const SizeF& AvailableSize,
    SizeF& UsedSize 
    )
{
    HRESULT hr = S_OK;   
    CThumb* pThumb = NULL;
    CButton* pIncreaseButton = NULL;
    CButton* pDecreaseButton = NULL;
    FLOAT ViewportSize = 0;
    Orientation::Value TrackOrientation;
    FLOAT DecreaseButtonLength = 0;
    FLOAT ThumbLength = 0;
    FLOAT IncreaseButtonLength = 0;

    IFC(GetTypedEffectiveValue(&ThumbProperty, &pThumb));

    IFC(GetTypedEffectiveValue(&IncreaseButtonProperty, &pIncreaseButton));
    IFC(GetTypedEffectiveValue(&DecreaseButtonProperty, &pDecreaseButton));

    IFC(GetBasicTypeEffectiveValue(&ViewportSizeProperty, &ViewportSize));

    IFC(GetBasicTypeEffectiveValue(&OrientationProperty, &TrackOrientation));

    if(_isnan(ViewportSize))
    {
        IFC(ComputeSliderLengths(AvailableSize, TrackOrientation, &DecreaseButtonLength, &ThumbLength, &IncreaseButtonLength));
    }
    else
    {
        ViewportSize = std::max(0.0f, ViewportSize);

        bool Hide = FALSE;

        IFC(ComputeScrollBarLengths(AvailableSize, ViewportSize, TrackOrientation, &DecreaseButtonLength, &ThumbLength, &IncreaseButtonLength, &Hide));

        if(Hide)
        {
            UsedSize = AvailableSize;

            goto Cleanup;
        }
    }

	{
		Point2F Offset;
		SizeF PieceSize = AvailableSize;
		bool IsDirectionReversed = FALSE;

		IFC(GetBasicTypeEffectiveValue(&DirectionReversedProperty, &IsDirectionReversed));

		if(TrackOrientation == Orientation::Vertical)
		{
			CoerceLength(DecreaseButtonLength, AvailableSize.height);
			CoerceLength(IncreaseButtonLength, AvailableSize.height);
			CoerceLength(ThumbLength, AvailableSize.height);

			Offset.y = IsDirectionReversed ? DecreaseButtonLength + ThumbLength : 0.0f;
			PieceSize.height = IncreaseButtonLength;
                
			if (pIncreaseButton != NULL)
			{
				IFC(pIncreaseButton->Arrange(MakeRect(Offset, PieceSize)));
			}

			Offset.y = IsDirectionReversed ? 0.0f : IncreaseButtonLength + ThumbLength;
			PieceSize.height = DecreaseButtonLength;

			if (pDecreaseButton != NULL)
			{
				IFC(pDecreaseButton->Arrange(MakeRect(Offset, PieceSize)));
			}

			Offset.y = IsDirectionReversed ? DecreaseButtonLength : IncreaseButtonLength;
			PieceSize.height = ThumbLength;

			if (pThumb != NULL)
			{
				IFC(pThumb->Arrange(MakeRect(Offset, PieceSize)));
			}

			m_ThumbCenterOffset = Offset.y + (ThumbLength * 0.5f);
		}
		else
		{
			CoerceLength(DecreaseButtonLength, AvailableSize.width);
			CoerceLength(IncreaseButtonLength, AvailableSize.width);
			CoerceLength(ThumbLength, AvailableSize.width);

			Offset.x = IsDirectionReversed ? IncreaseButtonLength + ThumbLength : 0.0f;
			PieceSize.width = DecreaseButtonLength;
                
			if (pDecreaseButton != NULL)
			{
				IFC(pDecreaseButton->Arrange(MakeRect(Offset, PieceSize)));
			}

			Offset.x = IsDirectionReversed ? 0.0f : DecreaseButtonLength + ThumbLength;
			PieceSize.width = IncreaseButtonLength;

			if (pIncreaseButton != NULL)
			{
				IFC(pIncreaseButton->Arrange(MakeRect(Offset, PieceSize)));
			}

			Offset.x = IsDirectionReversed ? IncreaseButtonLength : DecreaseButtonLength;
			PieceSize.width = ThumbLength;

			if (pThumb != NULL)
			{
				IFC(pThumb->Arrange(MakeRect(Offset, PieceSize)));
			}

			m_ThumbCenterOffset = Offset.x + (ThumbLength * 0.5f);
		}
	}

    UsedSize = AvailableSize;

Cleanup:
    ReleaseObject(pThumb);
    ReleaseObject(pIncreaseButton);
    ReleaseObject(pDecreaseButton);

    return hr;
}