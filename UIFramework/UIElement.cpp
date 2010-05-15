#include "UIElement.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "DelegatingPropertyInformation.h"
#include "MouseInput.h"
#include "RoutedEventInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Width );
DEFINE_GET_DEFAULT_NULL( Height );
DEFINE_GET_DEFAULT( MinimumWidth, CFloatValue, 0 );
DEFINE_GET_DEFAULT( MinimumHeight, CFloatValue, 0 );
DEFINE_GET_DEFAULT( MaximumWidth, CFloatValue, FLT_MAX );
DEFINE_GET_DEFAULT( MaximumHeight, CFloatValue, FLT_MAX );
DEFINE_GET_DEFAULT( Visibility, CVisibilityValue, Visibility::Visible );
DEFINE_GET_DEFAULT( HorizontalAlignment, CHorizontalAlignmentValue, HorizontalAlignment::Stretch );
DEFINE_GET_DEFAULT( VerticalAlignment, CVerticalAlignmentValue, VerticalAlignment::Stretch );

//
// Properties
//
CStaticProperty CUIElement::WidthProperty( L"Width", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Width ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnWidthChanged ) );
CStaticProperty CUIElement::HeightProperty( L"Height", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Height ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnHeightChanged ) );
CStaticProperty CUIElement::MinimumWidthProperty( L"MinimumWidth", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( MinimumWidth ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnMinimumWidthChanged ) );
CStaticProperty CUIElement::MinimumHeightProperty( L"MinimumHeight", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( MinimumHeight ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnMinimumHeightChanged ) );
CStaticProperty CUIElement::MaximumWidthProperty( L"MaximumWidth", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( MaximumWidth ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnMaximumWidthChanged ) );
CStaticProperty CUIElement::MaximumHeightProperty( L"MaximumHeight", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( MaximumHeight ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnMaximumHeightChanged ) );
CStaticProperty CUIElement::VisibilityProperty( L"Visibility", TypeIndex::Visibility, StaticPropertyFlags::None, &GET_DEFAULT( Visibility ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnVisibilityChanged ) );
CStaticProperty CUIElement::HorizontalAlignmentProperty( L"HorizontalAlignment", TypeIndex::HorizontalAlignment, StaticPropertyFlags::None, &GET_DEFAULT( HorizontalAlignment ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnHorizontalAlignmentChanged ) );
CStaticProperty CUIElement::VerticalAlignmentProperty( L"VerticalAlignment", TypeIndex::VerticalAlignment, StaticPropertyFlags::None, &GET_DEFAULT( VerticalAlignment ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnVerticalAlignmentChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnWidthChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnHeightChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnMinimumWidthChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnMinimumHeightChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnMaximumWidthChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnMaximumHeightChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnVisibilityChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnHorizontalAlignmentChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnVerticalAlignmentChanged );

//
// Events
//
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::AttachedEvent(L"Attached");
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::DetachedEvent(L"Detached");

CStaticRoutedEvent< RoutingStrategy::Bubbling > CUIElement::MouseButtonEvent(L"MouseButton");

CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseDownEvent(L"MouseDown");
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseUpEvent(L"MouseUp");

CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseLeftButtonDownEvent(L"MouseLeftButtonDown");
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseRightButtonDownEvent(L"MouseRightButtonDown");
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseMiddleButtonDownEvent(L"MouseMiddleButtonDown");

CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseLeftButtonUpEvent(L"MouseLeftButtonUp");
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseRightButtonUpEvent(L"MouseRightButtonUp");
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseMiddleButtonUpEvent(L"MouseMiddleButtonUp");

CStaticRoutedEvent< RoutingStrategy::Bubbling > CUIElement::MouseMoveEvent(L"MouseMove");

CUIElement::CUIElement() : m_Attached(FALSE),
                           m_MeasureDirty(TRUE),
                           m_ArrangeDirty(TRUE),
                           m_PropertyInformation(NULL),
                           m_Width(this, &CUIElement::WidthProperty),
                           m_Height(this, &CUIElement::HeightProperty),
                           m_MinimumWidth(this, &CUIElement::MinimumWidthProperty),
                           m_MinimumHeight(this, &CUIElement::MinimumHeightProperty),
                           m_MaximumWidth(this, &CUIElement::MaximumWidthProperty),
                           m_MaximumHeight(this, &CUIElement::MaximumHeightProperty),
                           m_Visibility(this, &CUIElement::VisibilityProperty),
                           m_VerticalAlignment(this, &CUIElement::VerticalAlignmentProperty),
                           m_HorizontalAlignment(this, &CUIElement::HorizontalAlignmentProperty)
{
    m_DesiredSize.width = 0;
    m_DesiredSize.height = 0;

    m_LastMeasureSize.width = 0;
    m_LastMeasureSize.height = 0;

    m_LastArrangeBounds.left = 0;
    m_LastArrangeBounds.top = 0;
    m_LastArrangeBounds.right = 0;
    m_LastArrangeBounds.bottom = 0;

    m_FinalSize.width = 0;
    m_FinalSize.height = 0;
}

CUIElement::~CUIElement()
{
    m_MouseButtonConnection.disconnect();
    m_MouseDownConnection.disconnect();
    m_MouseUpConnection.disconnect();
    m_MouseLeftButtonDownConnection.disconnect();
    m_MouseRightButtonDownConnection.disconnect();
    m_MouseMiddleButtonDownConnection.disconnect();
    m_MouseLeftButtonUpConnection.disconnect();
    m_MouseRightButtonUpConnection.disconnect();
    m_MouseMiddleButtonUpConnection.disconnect();
    m_MouseMoveConnection.disconnect();

    ReleaseObject(m_PropertyInformation);

    for(std::vector< CEventHandlerChain* >::iterator It = m_EventHandlers.begin(); It != m_EventHandlers.end(); ++It)
    {
        (*It)->Release();
    }

    m_EventHandlers.clear();

    ReleaseObject(m_Providers);
}

HRESULT CUIElement::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

    IFC(CVisual::Initialize());

    IFC(AddHandler(&MouseButtonEvent, boost::bind(&CUIElement::OnMouseButton, this, _1, _2), &m_MouseButtonConnection));

    IFC(AddHandler(&MouseDownEvent, boost::bind(&CUIElement::OnMouseDown, this, _1, _2), &m_MouseDownConnection));
    IFC(AddHandler(&MouseUpEvent, boost::bind(&CUIElement::OnMouseUp, this, _1, _2), &m_MouseUpConnection));

    IFC(AddHandler(&MouseLeftButtonDownEvent, boost::bind(&CUIElement::OnMouseLeftButtonDown, this, _1, _2), &m_MouseLeftButtonDownConnection));
    IFC(AddHandler(&MouseRightButtonDownEvent, boost::bind(&CUIElement::OnMouseRightButtonDown, this, _1, _2), &m_MouseRightButtonDownConnection));
    IFC(AddHandler(&MouseMiddleButtonDownEvent, boost::bind(&CUIElement::OnMouseMiddleButtonDown, this, _1, _2), &m_MouseMiddleButtonDownConnection));

    IFC(AddHandler(&MouseRightButtonUpEvent, boost::bind(&CUIElement::OnMouseRightButtonUp, this, _1, _2), &m_MouseRightButtonUpConnection));
    IFC(AddHandler(&MouseLeftButtonUpEvent, boost::bind(&CUIElement::OnMouseLeftButtonUp, this, _1, _2), &m_MouseLeftButtonUpConnection));
    IFC(AddHandler(&MouseMiddleButtonUpEvent, boost::bind(&CUIElement::OnMouseMiddleButtonUp, this, _1, _2), &m_MouseMiddleButtonUpConnection));

    IFC(AddHandler(&MouseMoveEvent, boost::bind(&CUIElement::OnMouseMove, this, _1, _2), &m_MouseMoveConnection));
    
Cleanup:
    return hr;
}

HRESULT CUIElement::PreRender(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;
    Visibility::Value EffectiveVisibility = Visibility::Visible;

    IFC(GetEffectiveVisibility(&EffectiveVisibility));

    if(EffectiveVisibility == Visibility::Visible)
    {
        IFC(PreRenderInternal(Context));
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CVisual::PreRender(Context));

Cleanup:
    return hr;
}

HRESULT CUIElement::Render(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    Visibility::Value EffectiveVisibility = Visibility::Visible;

    IFC(GetEffectiveVisibility(&EffectiveVisibility));

    if(EffectiveVisibility == Visibility::Visible)
    {
        IFC(RenderInternal(Context));
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::RenderInternal(CRenderContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CVisual::Render(Context));

Cleanup:
    return hr;
}

HRESULT CUIElement::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pAttachedEventArgs = NULL;

    IFCEXPECT(!IsAttached());

    m_Attached = TRUE;
    m_Context = Context;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());

    IFC(CRoutedEventArgs::Create(&CUIElement::AttachedEvent, &pAttachedEventArgs));

    IFC(RaiseEvent(pAttachedEventArgs));
    
Cleanup:
    ReleaseObject(pAttachedEventArgs);

    return hr;
}

HRESULT CUIElement::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pDetachedEventArgs = NULL;

    IFCEXPECT(IsAttached());

    m_Attached = FALSE;

    m_Context.Reset();
    
    IFC(CRoutedEventArgs::Create(&CUIElement::DetachedEvent, &pDetachedEventArgs));

    IFC(RaiseEvent(pDetachedEventArgs));
    
Cleanup:
    ReleaseObject(pDetachedEventArgs);

    return hr;
}

BOOL CUIElement::IsAttached()
{
    return m_Attached;
}

HRESULT CUIElement::SetSize(SizeF Size)
{
    HRESULT hr = S_OK;

    //IFC(InternalSetWidth(Size.width));
    //IFC(InternalSetHeight(Size.height));

Cleanup:
    return hr;
}

HRESULT CUIElement::GetMinMaxSize(SizeF& MinimumSize, SizeF& MaximumSize)
{
    HRESULT hr = S_OK;
    CFloatValue* pHeight = NULL;
    CFloatValue* pWidth = NULL;

    IFC(m_Height.GetTypedEffectiveValue(GetProviders(), &pHeight));

    FLOAT MaxHeight = 0;
    FLOAT MinHeight = 0;

    IFC(GetEffectiveMaximumHeight(&MaxHeight));
    IFC(GetEffectiveMinimumHeight(&MinHeight));

    FLOAT Height = (pHeight != NULL) ? pHeight->GetValue() : FLT_MAX;
    MaxHeight = max(min(Height, MaxHeight), MinHeight);

    Height = (pHeight != NULL) ? pHeight->GetValue() : 0;
    MinHeight = max(min(MaxHeight, Height), MinHeight);

    FLOAT MaxWidth = 0;
    FLOAT MinWidth = 0;

    IFC(GetEffectiveMaximumWidth(&MaxWidth));
    IFC(GetEffectiveMinimumWidth(&MinWidth));

    IFC(m_Width.GetTypedEffectiveValue(GetProviders(), &pWidth));

    FLOAT Width = (pWidth != NULL) ? pWidth->GetValue() : FLT_MAX;
    MaxWidth = max(min(Width, MaxWidth), MinWidth);

    Width = (pWidth != NULL) ? pWidth->GetValue() : 0;
    MinWidth = max(min(MaxWidth, Width), MinWidth);

    MinimumSize.width = MinWidth;
    MinimumSize.height = MinHeight;

    MaximumSize.width = MaxWidth;
    MaximumSize.height = MaxHeight;

Cleanup:
    ReleaseObject(pWidth);
    ReleaseObject(pHeight);

    return hr;
}

HRESULT CUIElement::Measure(SizeF AvailableSize)
{
    HRESULT hr = S_OK;

    if(m_MeasureDirty || m_LastMeasureSize.height != AvailableSize.height || m_LastMeasureSize.width != AvailableSize.width)
    {
        Visibility::Value EffectiveVisibility = Visibility::Visible;

        IFC(GetEffectiveVisibility(&EffectiveVisibility));

        if(EffectiveVisibility == Visibility::Visible || EffectiveVisibility == Visibility::Hidden)
        {
            SizeF ElementAvailableSize = { max(AvailableSize.width /* - MarginWidth */, 0), max(AvailableSize.height /* - MarginHeight */, 0) };
            SizeF MinSize = { 0 };
            SizeF MaxSize = { 0 };

            IFC(GetMinMaxSize(MinSize, MaxSize));

            ElementAvailableSize.width = max(MinSize.width, min(ElementAvailableSize.width, MaxSize.width));
            ElementAvailableSize.height = max(MinSize.height, min(ElementAvailableSize.height, MaxSize.height));

            SizeF ElementDesiredSize = { 0 };

            IFC(MeasureInternal(ElementAvailableSize, ElementDesiredSize));            

            ElementDesiredSize.width = max(ElementDesiredSize.width, MinSize.width);
            ElementDesiredSize.height = max(ElementDesiredSize.height, MinSize.height);

            m_UnclippedDesiredSize = ElementDesiredSize;

            BOOL Clipped = FALSE;

            if(ElementDesiredSize.width > MaxSize.width)
            {
                ElementDesiredSize.width = MaxSize.width;
                Clipped = TRUE;
            }

            if(ElementDesiredSize.height > MaxSize.height)
            {
                ElementDesiredSize.height = MaxSize.height;
                Clipped = TRUE;
            }

            FLOAT ClippedDesiredWidth = ElementDesiredSize.width /*+ MarginWidth */;
            FLOAT ClippedDesiredHeight = ElementDesiredSize.height /*+ MarginHeight*/;

            if (ClippedDesiredWidth > AvailableSize.width)
            {
                ClippedDesiredWidth = AvailableSize.width;
                Clipped = TRUE;
            }

            if (ClippedDesiredHeight > AvailableSize.height)
            {
                ClippedDesiredHeight = AvailableSize.height;
                Clipped = TRUE;
            }

            m_DesiredSize.width = max(0, ClippedDesiredWidth);
            m_DesiredSize.height = max(0, ClippedDesiredHeight);

            IFC(InvalidateArrange());
        }
        else
        {
            m_DesiredSize.width = 0;
            m_DesiredSize.height = 0;
        }

        m_MeasureDirty = FALSE;
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;

    DesiredSize.width = 0;
    DesiredSize.height = 0;

Cleanup:
    return hr;
}

HRESULT CUIElement::GetEffectiveWidth(FLOAT* pWidth)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pWidth);

    IFC(m_Width.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pWidth = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveHeight(FLOAT* pHeight)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pHeight);

    IFC(m_Height.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pHeight = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveVisibility(Visibility::Value* pVisibility)
{
    HRESULT hr = S_OK;
    CVisibilityValue* pEffectiveValue = NULL;

    IFCPTR(pVisibility);

    IFC(m_Visibility.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pVisibility = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveMinimumWidth(FLOAT* pMinimumWidth)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pMinimumWidth);

    IFC(m_MinimumWidth.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pMinimumWidth = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveMinimumHeight(FLOAT* pMinimumHeight)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pMinimumHeight);

    IFC(m_MinimumHeight.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pMinimumHeight = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveMaximumWidth(FLOAT* pMaximumWidth)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pMaximumWidth);

    IFC(m_MaximumWidth.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pMaximumWidth = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveMaximumHeight(FLOAT* pMaximumHeight)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pMaximumHeight);

    IFC(m_MaximumHeight.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pMaximumHeight = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveHorizontalAlignment(HorizontalAlignment::Value* pAlignment)
{
    HRESULT hr = S_OK;
    CHorizontalAlignmentValue* pEffectiveValue = NULL;

    IFCPTR(pAlignment);

    IFC(m_HorizontalAlignment.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pAlignment = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveVerticalAlignment(VerticalAlignment::Value* pAlignment)
{
    HRESULT hr = S_OK;
    CVerticalAlignmentValue* pEffectiveValue = NULL;

    IFCPTR(pAlignment);

    IFC(m_VerticalAlignment.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pAlignment = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

SizeF CUIElement::GetDesiredSize()
{
    return m_DesiredSize;
}

SizeF CUIElement::GetFinalSize()
{
    return m_FinalSize;
}

HRESULT CUIElement::Arrange(RectF Bounds)
{
    HRESULT hr = S_OK;

    if(m_ArrangeDirty || Bounds != m_LastArrangeBounds)
    {
        HorizontalAlignment::Value HorzAlign;
        VerticalAlignment::Value VertAlign;

        m_LastArrangeBounds = Bounds;

        Visibility::Value EffectiveVisibility = Visibility::Visible;

        IFC(GetEffectiveVisibility(&EffectiveVisibility))

        if(EffectiveVisibility == Visibility::Visible || EffectiveVisibility == Visibility::Hidden)
        {
            BOOL NeedsClipBounds = FALSE;

            SizeF ArrangeSize = { Bounds.right - Bounds.left, Bounds.bottom - Bounds.top };

            FLOAT MarginWidth = 0; /*margin.Left + margin.Right;*/
            FLOAT MarginHeight = 0; /*margin.Top + margin.Bottom;*/

            ArrangeSize.width = max(0, ArrangeSize.width - MarginWidth);
            ArrangeSize.height = max(0, ArrangeSize.height - MarginHeight);

            if(ArrangeSize.width < m_UnclippedDesiredSize.width)
            {
                NeedsClipBounds = TRUE;
                ArrangeSize.width = m_UnclippedDesiredSize.width;
            }

            if(ArrangeSize.height < m_UnclippedDesiredSize.height)
            {
                NeedsClipBounds = TRUE;
                ArrangeSize.height = m_UnclippedDesiredSize.height;
            }

            IFC(GetEffectiveHorizontalAlignment(&HorzAlign));
            IFC(GetEffectiveVerticalAlignment(&VertAlign));

            if (HorzAlign != HorizontalAlignment::Stretch)
            {
                ArrangeSize.width = m_UnclippedDesiredSize.width;
            }

            if (VertAlign != VerticalAlignment::Stretch)
            {
                ArrangeSize.height = m_UnclippedDesiredSize.height;
            }

            SizeF MinSize = { 0 };
            SizeF MaxSize = { 0 };

            IFC(GetMinMaxSize(MinSize, MaxSize));      

            FLOAT EffectiveMaxWidth = max(m_UnclippedDesiredSize.width, MaxSize.width);

            if(EffectiveMaxWidth < ArrangeSize.width)
            {
                NeedsClipBounds = TRUE;
                ArrangeSize.width = EffectiveMaxWidth;
            }

            FLOAT EffectiveMaxHeight = max(m_UnclippedDesiredSize.height, MaxSize.height);

            if(EffectiveMaxHeight < ArrangeSize.height)
            {
                NeedsClipBounds = true;
                ArrangeSize.height = EffectiveMaxHeight;
            }

            SizeF UsedArrangeSize = { 0 };

            IFC(ArrangeInternal(ArrangeSize, UsedArrangeSize));

            SizeF ClippedSize = { min(UsedArrangeSize.width, MaxSize.width), min(UsedArrangeSize.height, MaxSize.height) };

            NeedsClipBounds |= (ClippedSize.width < UsedArrangeSize.width) ||  (ClippedSize.height < UsedArrangeSize.height);

            SizeF ClientSize = { max(0, ArrangeSize.width - MarginWidth), max(0, ArrangeSize.height - MarginHeight) };

            NeedsClipBounds |= (ClientSize.width < ClippedSize.width) || (ClientSize.height < ClippedSize.height);

            SizeF Offset = { 0 };

            IFC(ComputeAlignmentOffset(ClientSize, ClippedSize, Offset));

            Offset.width += Bounds.left /*+ margin.Left*/;
            Offset.height += Bounds.top /*+ margin.Top*/;

            Matrix3X2 VisualTransform = D2D1::Matrix3x2F::Translation(Offset);
            
            //TODO: Use something other than visual transform?
            IFC(SetVisualTransform(VisualTransform));

            m_FinalSize = ClippedSize;

            m_ArrangeDirty = FALSE;
        }
        else
        {
            m_FinalSize.width = 0;
            m_FinalSize.height = 0;
        }
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::ComputeAlignmentOffset(SizeF ClientSize, SizeF RenderSize, SizeF Offset)
{
    HRESULT hr = S_OK;

    HorizontalAlignment::Value HorzAlign;
    VerticalAlignment::Value VertAlign;

    IFC(GetEffectiveHorizontalAlignment(&HorzAlign));
    IFC(GetEffectiveVerticalAlignment(&VertAlign));

    if(HorzAlign == HorizontalAlignment::Stretch && RenderSize.width > ClientSize.width)
    {
        HorzAlign = HorizontalAlignment::Left;
    }

    if(VertAlign == VerticalAlignment::Stretch && RenderSize.height > ClientSize.height)
    {
        VertAlign = VerticalAlignment::Top;
    }

    if(HorzAlign == HorizontalAlignment::Center ||  HorzAlign == HorizontalAlignment::Stretch)
    {
        Offset.width = (ClientSize.width - RenderSize.width) * 0.5;
    }
    else if(HorzAlign == HorizontalAlignment::Right)
    {
        Offset.width = ClientSize.width - RenderSize.width;
    }
    else
    {
        Offset.width = 0;
    }

    if(VertAlign == VerticalAlignment::Center ||  VertAlign == VerticalAlignment::Stretch)
    {
        Offset.height = (ClientSize.height - RenderSize.height) * 0.5;
    }
    else if(VertAlign == VerticalAlignment::Bottom)
    {
        Offset.height = ClientSize.height - RenderSize.height;
    }
    else
    {
        Offset.height = 0;
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;

    UsedSize.width = 0;
    UsedSize.height = 0;

    return hr;
}

HRESULT CUIElement::InvalidateMeasure()
{
    HRESULT hr = S_OK;
    CChildMeasureInvalidatedNotification* pNotification = NULL;

    if(!m_MeasureDirty)
    {
        m_MeasureDirty = TRUE;

        IFC(CChildMeasureInvalidatedNotification::Create(&pNotification));

        IFC(NotifyParent(pNotification));
    }

Cleanup:
    ReleaseObject(pNotification);

    return hr;
}

HRESULT CUIElement::InvalidateArrange()
{
    HRESULT hr = S_OK;
    CChildArrangeInvalidatedNotification* pNotification = NULL;

    if(!m_ArrangeDirty)
    {
        m_ArrangeDirty = TRUE;

        IFC(CChildArrangeInvalidatedNotification::Create(&pNotification));

        IFC(NotifyParent(pNotification));
    }

Cleanup:
    ReleaseObject(pNotification);

    return hr;
}

BOOL CUIElement::IsMeasureDirty()
{
    return m_MeasureDirty;
}

BOOL CUIElement::IsArrangeDirty()
{
    return m_ArrangeDirty;
}

CUIElement* CUIElement::GetParent()
{
    return m_Context.GetParent();
}

CUIElement* CUIElement::GetTemplateParent()
{
    return m_Context.GetTemplateParent();
}

CProviders* CUIElement::GetProviders()
{
    return m_Providers;
}

CTypeConverter* CUIElement::GetTypeConverter()
{
    return GetProviders()->GetTypeConverter();
}

HRESULT CUIElement::NotifyParent(CUINotification* pNotification)
{
    HRESULT hr = S_OK;
    CUIElement* pParent = NULL;

    IFCPTR(pNotification);

    pParent = GetParent();

    if(pParent)
    {
        IFC(pParent->OnUINotification(pNotification));
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::OnUINotification(CUINotification* pNotification)
{
    HRESULT hr = S_OK;

    IFCPTR(pNotification);

    switch(pNotification->GetType())
    {
        case UINotification::ChildMeasureInvalidated:
            {
                CChildMeasureInvalidatedNotification* pMeasureInvalidationNotification = (CChildMeasureInvalidatedNotification*)pNotification;
                IFC(OnChildMeasureInvalidated(pMeasureInvalidationNotification));
                break;
            }

        case UINotification::ChildArrangeInvalidated:
            {
                CChildArrangeInvalidatedNotification* pArrangeInvalidationNotification = (CChildArrangeInvalidatedNotification*)pNotification;
                IFC(OnChildArrangeInvalidated(pArrangeInvalidationNotification));
                break;
            }
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::SetVisibility(Visibility::Value State)
{
    HRESULT hr = S_OK;
    CVisibilityValue* pValue = NULL;

    IFC(CVisibilityValue::Create(State, &pValue));

    IFC(SetValue(&CUIElement::VisibilityProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

HRESULT CUIElement::OnChildMeasureInvalidated(CChildMeasureInvalidatedNotification* pNotification)
{
    HRESULT hr = S_OK;

    IFCPTR(pNotification);

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnChildArrangeInvalidated(CChildArrangeInvalidatedNotification* pNotification)
{
    HRESULT hr = S_OK;

    IFCPTR(pNotification);

    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CUIElement::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &WidthProperty,
        &HeightProperty,
        &MinimumWidthProperty,
        &MinimumHeightProperty,
        &MaximumWidthProperty,
        &MaximumHeightProperty,
        &VisibilityProperty,
        &HorizontalAlignmentProperty,
        &VerticalAlignmentProperty,
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CVisual::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CUIElement::CreateEventInformation(CEventInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CRoutedEventInformation* pEventInformation = NULL;

    IFCPTR(ppInformation);

    CRoutedEvent* Events[] = 
    {
        &MouseButtonEvent,
        &MouseDownEvent,
        &MouseLeftButtonDownEvent,
        &MouseRightButtonDownEvent,
        &MouseMiddleButtonDownEvent,
        &MouseLeftButtonUpEvent,
        &MouseRightButtonUpEvent,
        &MouseMiddleButtonUpEvent,
        &MouseMoveEvent
    };

    IFC(CRoutedEventInformation::Create(Events, ARRAYSIZE(Events), &pEventInformation));

    *ppInformation = pEventInformation;
    pEventInformation = NULL;

Cleanup:
    ReleaseObject(pEventInformation);

    return hr;
}

HRESULT CUIElement::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CUIElement::WidthProperty)
    {
        *ppLayeredValue = &m_Width;
    }
    else if(pProperty == &CUIElement::HeightProperty)
    {
        *ppLayeredValue = &m_Height;
    }
    else if(pProperty == &CUIElement::MinimumWidthProperty)
    {
        *ppLayeredValue = &m_MinimumWidth;
    }
    else if(pProperty == &CUIElement::MinimumHeightProperty)
    {
        *ppLayeredValue = &m_MinimumHeight;
    }
    else if(pProperty == &CUIElement::MaximumWidthProperty)
    {
        *ppLayeredValue = &m_MinimumWidth;
    }
    else if(pProperty == &CUIElement::MaximumHeightProperty)
    {
        *ppLayeredValue = &m_MaximumHeight;
    }
    else if(pProperty == &CUIElement::VisibilityProperty)
    {
        *ppLayeredValue = &m_Visibility;
    }
    else if(pProperty == &CUIElement::HorizontalAlignmentProperty)
    {
        *ppLayeredValue = &m_HorizontalAlignment;
    }
    else if(pProperty == &CUIElement::VerticalAlignmentProperty)
    {
        *ppLayeredValue = &m_VerticalAlignment;
    }
    else
    {
        hr = E_FAIL;
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFCEXPECT(!pProperty->IsReadOnly());

    if(pProperty->IsAttached())
    {
        //TODO: Get or create layered value holder here and set it.
        if(SUCCEEDED(E_FAIL))
        {
        }
        else
        {
            IFC(CVisual::SetValueInternal(pProperty, pValue));
        }
    }
    else
    {
        IFC(GetLayeredValue(pProperty, &pLayeredValue));

        IFC(pLayeredValue->SetLocalValue(pValue, GetProviders()));

        //NOTE: Don't call the base class here as all properties are expected to be layered.
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty->IsAttached())
    {
        //TODO: Get layered value holder here and get value.
        if(SUCCEEDED(E_FAIL))
        {
        }
        else
        {
            IFC(GetValueInternal(pProperty, ppValue));
        }
    }
    else
    {
        IFC(GetLayeredValue(pProperty, &pLayeredValue));

        IFC(pLayeredValue->GetLocalValue(GetProviders(), ppValue));

        //NOTE: Don't call the base class here as all properties are expected to be layered.
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    IFC(CVisual::GetValue(pProperty, ppValue));

Cleanup:
    return hr;
}

HRESULT CUIElement::GetEffectiveValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)))
    {
        IFC(pLayeredValue->GetEffectiveValue(GetProviders(), ppValue));
    }
    else
    {
        //TODO: This will trigger yet another layered property lookup, needs optomization.
        IFC(GetValue(pProperty, ppValue));
    }

Cleanup:

    return hr;
}

HRESULT CUIElement::OnWidthChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnHeightChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnMinimumWidthChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnMinimumHeightChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnMaximumWidthChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnMaximumHeightChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnVisibilityChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnHorizontalAlignmentChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnVerticalAlignmentChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CUIElement::RaiseEvent(CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CRoutedEvent* pRoutedEvent = NULL;

    IFCPTR(pRoutedEventArgs);

    pRoutedEvent = pRoutedEventArgs->GetRoutedEvent();
    IFCPTR(pRoutedEvent);

    switch(pRoutedEvent->GetRoutingStrategy())
    {
        case RoutingStrategy::Direct:
            {
                IFC(InternalRaiseEvent(pRoutedEventArgs));
                
                break;
            }

        case RoutingStrategy::Bubbling:
            {
                IFC(InternalRaiseBubbledEvent(pRoutedEventArgs));

                break;
            }

        default:
            {
                IFC(E_UNEXPECTED);
            }
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::InternalRaiseEvent(CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEventArgs);

    for(std::vector< CEventHandlerChain* >::iterator It = m_EventHandlers.begin(); It != m_EventHandlers.end(); ++It)
    {
        CEventHandlerChain* pCurrentChain = (*It);

        if(pCurrentChain->GetRoutedEvent() == pRoutedEventArgs->GetRoutedEvent())
        {
            IFC(pCurrentChain->RaiseEvent(this, pRoutedEventArgs));

            break;
        }
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::InternalRaiseBubbledEvent(CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CUIElement* pParent = NULL;

    IFCPTR(pRoutedEventArgs);

    IFC(InternalRaiseEvent(pRoutedEventArgs));

    if(!pRoutedEventArgs->IsHandled())
    {
        pParent = GetParent();

        if(pParent)
        {
            IFC(pParent->InternalRaiseBubbledEvent(pRoutedEventArgs));
        }
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::AddHandler(CRoutedEvent* pRoutedEvent, const RoutedEventHandler& Handler, connection* pConnection)
{
    HRESULT hr = S_OK;
    CEventHandlerChain* pHandlerChain = NULL;

    IFCPTR(pRoutedEvent);
    IFCPTR(pConnection);

    for(std::vector< CEventHandlerChain* >::iterator It = m_EventHandlers.begin(); It != m_EventHandlers.end(); ++It)
    {
        CEventHandlerChain* pCurrentChain = (*It);

        if(pCurrentChain->GetRoutedEvent() == pRoutedEvent)
        {
            pHandlerChain = (*It);
            break;
        }
    }

    if(pHandlerChain == NULL)
    {
        IFC(CEventHandlerChain::Create(pRoutedEvent, &pHandlerChain));

        m_EventHandlers.push_back(pHandlerChain);
    }

    IFC(pHandlerChain->AddHandler(Handler, pConnection));

Cleanup:

    return hr;
}

void CUIElement::OnMouseButton(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CMouseButtonEventArgs* pMouseButtonEventArgs = NULL;
    CMouseButtonEventArgs* pNewEventArgs = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFCEXPECT(pRoutedEventArgs->IsTypeOf(TypeIndex::MouseButtonEventArgs));

    pMouseButtonEventArgs = (CMouseButtonEventArgs*)pRoutedEventArgs;

    if(!pMouseButtonEventArgs->IsHandled())
    {
        switch(pMouseButtonEventArgs->GetButtonState())
        {
            case MouseButtonState::Down:
                {
                    IFC(CMouseButtonEventArgs::Create(&MouseDownEvent, pMouseButtonEventArgs, &pNewEventArgs));

                    IFC(RaiseEvent(pNewEventArgs));

                    if(pNewEventArgs->IsHandled())
                    {
                        pMouseButtonEventArgs->SetHandled();
                    }

                    break;
                }

            case MouseButtonState::Up:
                {
                    IFC(CMouseButtonEventArgs::Create(&MouseUpEvent, pMouseButtonEventArgs, &pNewEventArgs));

                    IFC(RaiseEvent(pNewEventArgs));

                    if(pNewEventArgs->IsHandled())
                    {
                        pMouseButtonEventArgs->SetHandled();
                    }

                    break;
                }
        }
    }

Cleanup:
    ReleaseObject(pNewEventArgs);
    ;
}

void CUIElement::OnMouseDown(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CMouseButtonEventArgs* pMouseButtonEventArgs = NULL;
    CMouseButtonEventArgs* pNewEventArgs = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFCEXPECT(pRoutedEventArgs->IsTypeOf(TypeIndex::MouseButtonEventArgs));

    pMouseButtonEventArgs = (CMouseButtonEventArgs*)pRoutedEventArgs;

    if(!pMouseButtonEventArgs->IsHandled())
    {
        switch(pMouseButtonEventArgs->GetChangedButton())
        {
            case MouseButton::Left:
                {
                    IFC(CMouseButtonEventArgs::Create(&MouseLeftButtonDownEvent, pMouseButtonEventArgs, &pNewEventArgs));

                    IFC(RaiseEvent(pNewEventArgs));

                    if(pNewEventArgs->IsHandled())
                    {
                        pMouseButtonEventArgs->SetHandled();
                    }

                    break;
                }

            case MouseButton::Right:
                {
                    IFC(CMouseButtonEventArgs::Create(&MouseRightButtonDownEvent, pMouseButtonEventArgs, &pNewEventArgs));

                    IFC(RaiseEvent(pNewEventArgs));

                    if(pNewEventArgs->IsHandled())
                    {
                        pMouseButtonEventArgs->SetHandled();
                    }

                    break;
                }

            case MouseButton::Middle:
                {
                    IFC(CMouseButtonEventArgs::Create(&MouseMiddleButtonDownEvent, pMouseButtonEventArgs, &pNewEventArgs));

                    IFC(RaiseEvent(pNewEventArgs));

                    if(pNewEventArgs->IsHandled())
                    {
                        pMouseButtonEventArgs->SetHandled();
                    }

                    break;
                }
        }
    }

Cleanup:
    ReleaseObject(pNewEventArgs);
    ;
}



void CUIElement::OnMouseUp(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CMouseButtonEventArgs* pMouseButtonEventArgs = NULL;
    CMouseButtonEventArgs* pNewEventArgs = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFCEXPECT(pRoutedEventArgs->IsTypeOf(TypeIndex::MouseButtonEventArgs));

    pMouseButtonEventArgs = (CMouseButtonEventArgs*)pRoutedEventArgs;

    if(!pMouseButtonEventArgs->IsHandled())
    {
        switch(pMouseButtonEventArgs->GetChangedButton())
        {
            case MouseButton::Left:
                {
                    IFC(CMouseButtonEventArgs::Create(&MouseLeftButtonUpEvent, pMouseButtonEventArgs, &pNewEventArgs));

                    IFC(RaiseEvent(pNewEventArgs));

                    if(pNewEventArgs->IsHandled())
                    {
                        pMouseButtonEventArgs->SetHandled();
                    }

                    break;
                }

            case MouseButton::Right:
                {
                    IFC(CMouseButtonEventArgs::Create(&MouseRightButtonUpEvent, pMouseButtonEventArgs, &pNewEventArgs));

                    IFC(RaiseEvent(pNewEventArgs));

                    if(pNewEventArgs->IsHandled())
                    {
                        pMouseButtonEventArgs->SetHandled();
                    }

                    break;
                }

            case MouseButton::Middle:
                {
                    IFC(CMouseButtonEventArgs::Create(&MouseMiddleButtonUpEvent, pMouseButtonEventArgs, &pNewEventArgs));

                    IFC(RaiseEvent(pNewEventArgs));

                    if(pNewEventArgs->IsHandled())
                    {
                        pMouseButtonEventArgs->SetHandled();
                    }

                    break;
                }
        }
    }

Cleanup:
    ReleaseObject(pNewEventArgs);
    ;
}

void CUIElement::OnMouseLeftButtonDown(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

Cleanup:
    ;
}

void CUIElement::OnMouseRightButtonDown(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

Cleanup:
    ;
}

void CUIElement::OnMouseMiddleButtonDown(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

Cleanup:
    ;
}

void CUIElement::OnMouseLeftButtonUp(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

Cleanup:
    ;
}

void CUIElement::OnMouseRightButtonUp(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

Cleanup:
    ;
}

void CUIElement::OnMouseMiddleButtonUp(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

Cleanup:
    ;
}

void CUIElement::OnMouseMove(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

Cleanup:
    ;
}

CEventHandlerChain::CEventHandlerChain() : m_RoutedEvent(NULL)
{
}

CEventHandlerChain::~CEventHandlerChain()
{
    ReleaseObject(m_RoutedEvent);
}

HRESULT CEventHandlerChain::Initialize(CRoutedEvent* pRoutedEvent)
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);

    m_RoutedEvent = pRoutedEvent;
    AddRefObject(m_RoutedEvent);

Cleanup:
    return hr;
}

CRoutedEvent* CEventHandlerChain::GetRoutedEvent()
{
    return m_RoutedEvent;
}

HRESULT CEventHandlerChain::AddHandler(const RoutedEventHandler& Handler, connection* pConnection)
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_Handlers.connect(Handler);

Cleanup:
    return hr;
}

BOOL CEventHandlerChain::HasHandlers()
{
    return !m_Handlers.empty();
}

HRESULT CEventHandlerChain::RaiseEvent(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    m_Handlers(pSender, pRoutedEventArgs);

Cleanup:
    return hr;
}