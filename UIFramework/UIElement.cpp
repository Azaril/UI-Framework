#include "UIElement.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "DelegatingPropertyInformation.h"
#include "MouseInput.h"
#include "RoutedEventInformation.h"
#include "FocusManager.h"
#include "KeyboardEventArgs.h"
#include "StaticCommandInformation.h"
#include "BindingManager.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "LayoutManager.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Width );
DEFINE_GET_DEFAULT_NULL( Height );
DEFINE_GET_DEFAULT( MinimumWidth, CFloatValue, 0 );
DEFINE_GET_DEFAULT( MinimumHeight, CFloatValue, 0 );
DEFINE_GET_DEFAULT( MaximumWidth, CFloatValue, std::numeric_limits< FLOAT >::max() );
DEFINE_GET_DEFAULT( MaximumHeight, CFloatValue, std::numeric_limits< FLOAT >::max() );
DEFINE_GET_DEFAULT( Visibility, CVisibilityValue, Visibility::Visible );
DEFINE_GET_DEFAULT( HorizontalAlignment, CHorizontalAlignmentValue, HorizontalAlignment::Stretch );
DEFINE_GET_DEFAULT( VerticalAlignment, CVerticalAlignmentValue, VerticalAlignment::Stretch );
DEFINE_GET_DEFAULT( Margin, CRectFValue, RectF(0, 0, 0, 0) );
DEFINE_GET_DEFAULT( Focusable, CBoolValue, FALSE );
DEFINE_GET_DEFAULT( Opacity, CFloatValue, 1.0f );
DEFINE_GET_DEFAULT( Namescope, CBoolValue, FALSE );

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
CStaticProperty CUIElement::MarginProperty( L"Margin", TypeIndex::RectF, StaticPropertyFlags::None, &GET_DEFAULT( Margin ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnMarginChanged ) );
CStaticProperty CUIElement::FocusableProperty( L"Focusable", TypeIndex::Bool, StaticPropertyFlags::None, &GET_DEFAULT( Focusable ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnFocusableChanged ) );
CStaticProperty CUIElement::OpacityProperty( L"Opacity", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( Opacity ), &INSTANCE_CHANGE_CALLBACK( CUIElement, OnOpacityChanged ) );
CStaticProperty CUIElement::NamescopeProperty( L"Namescope", TypeIndex::Bool, StaticPropertyFlags::None, &GET_DEFAULT( Namescope ) );

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
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnMarginChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnFocusableChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CUIElement, OnOpacityChanged );

//
// Events
//
CStaticRoutedEvent CUIElement::AttachedEvent(L"Attached", RoutingStrategy::Direct);
CStaticRoutedEvent CUIElement::DetachedEvent(L"Detached", RoutingStrategy::Direct);

CStaticRoutedEvent CUIElement::MouseButtonEvent(L"MouseButton", RoutingStrategy::Bubbling);

CStaticRoutedEvent CUIElement::MouseDownEvent(L"MouseDown", RoutingStrategy::Direct);
CStaticRoutedEvent CUIElement::MouseUpEvent(L"MouseUp", RoutingStrategy::Direct);

CStaticRoutedEvent CUIElement::MouseLeftButtonDownEvent(L"MouseLeftButtonDown", RoutingStrategy::Direct);
CStaticRoutedEvent CUIElement::MouseRightButtonDownEvent(L"MouseRightButtonDown", RoutingStrategy::Direct);
CStaticRoutedEvent CUIElement::MouseMiddleButtonDownEvent(L"MouseMiddleButtonDown", RoutingStrategy::Direct);

CStaticRoutedEvent CUIElement::MouseLeftButtonUpEvent(L"MouseLeftButtonUp", RoutingStrategy::Direct);
CStaticRoutedEvent CUIElement::MouseRightButtonUpEvent(L"MouseRightButtonUp", RoutingStrategy::Direct);
CStaticRoutedEvent CUIElement::MouseMiddleButtonUpEvent(L"MouseMiddleButtonUp", RoutingStrategy::Direct);

CStaticRoutedEvent CUIElement::MouseMoveEvent(L"MouseMove", RoutingStrategy::Bubbling);

CStaticRoutedEvent CUIElement::MouseEnterEvent(L"MouseEnter", RoutingStrategy::Bubbling);
CStaticRoutedEvent CUIElement::MouseLeaveEvent(L"MouseLeave", RoutingStrategy::Bubbling);

CStaticRoutedEvent CUIElement::PreviewGotFocusEvent(L"PreviewGotFocus", RoutingStrategy::Tunneled);
CStaticRoutedEvent CUIElement::GotFocusEvent(L"GotFocus", RoutingStrategy::Bubbling);

CStaticRoutedEvent CUIElement::PreviewLostFocusEvent(L"PreviewLostFocus", RoutingStrategy::Tunneled);
CStaticRoutedEvent CUIElement::LostFocusEvent(L"LostFocus", RoutingStrategy::Bubbling);

CStaticRoutedEvent CUIElement::KeyEvent(L"Key", RoutingStrategy::Bubbling);
CStaticRoutedEvent CUIElement::KeyDownEvent(L"KeyDown", RoutingStrategy::Direct);
CStaticRoutedEvent CUIElement::KeyUpEvent(L"KeyUp", RoutingStrategy::Direct);

CStaticRoutedEvent CUIElement::TextEvent(L"Text", RoutingStrategy::Bubbling);

CUIElement::CUIElement() : m_Attached(FALSE),
                           m_MeasureDirty(TRUE),
                           m_NotifiedParentMeasureDirty(FALSE),
                           m_ArrangeDirty(TRUE),
                           m_NotifiedParentArrangeDirty(FALSE),
                           m_Width(this, &CUIElement::WidthProperty),
                           m_Height(this, &CUIElement::HeightProperty),
                           m_MinimumWidth(this, &CUIElement::MinimumWidthProperty),
                           m_MinimumHeight(this, &CUIElement::MinimumHeightProperty),
                           m_MaximumWidth(this, &CUIElement::MaximumWidthProperty),
                           m_MaximumHeight(this, &CUIElement::MaximumHeightProperty),
                           m_Visibility(this, &CUIElement::VisibilityProperty),
                           m_VerticalAlignment(this, &CUIElement::VerticalAlignmentProperty),
                           m_HorizontalAlignment(this, &CUIElement::HorizontalAlignmentProperty),
                           m_Margin(this, &CUIElement::MarginProperty),
                           m_Focusable(this, &CUIElement::FocusableProperty),
                           m_Opacity(this, &CUIElement::OpacityProperty),
                           m_Namescope(this, &CUIElement::NamescopeProperty),
                           m_Layer(NULL),
                           m_ClipToLayoutBounds(FALSE)
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

    m_LayoutTransformOffset.width = 0;
    m_LayoutTransformOffset.height = 0;
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
    m_KeyConnection.disconnect();

    for(std::vector< CEventHandlerChain* >::iterator It = m_EventHandlers.begin(); It != m_EventHandlers.end(); ++It)
    {
        (*It)->Release();
    }

    m_EventHandlers.clear();

    ReleaseObject(m_Layer);

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

    IFC(AddHandler(&KeyEvent, boost::bind(&CUIElement::OnKey, this, _1, _2), &m_KeyConnection));
    
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

HRESULT CUIElement::RequiresLayer(BOOL* pRequiresLayer)
{
    HRESULT hr = S_OK;
    RectF ClipRect;
    CGraphicsGeometry* pClipGeometry = NULL;
    Visibility::Value EffectiveVisibility = Visibility::Visible;
    FLOAT Opacity = 0;

    IFCPTR(pRequiresLayer);

    *pRequiresLayer = FALSE;

    IFC(GetEffectiveVisibility(&EffectiveVisibility));

    if(EffectiveVisibility != Visibility::Visible)
    {
        goto Cleanup;
    }

    IFC(GetEffectiveOpacity(&Opacity));

    if(Opacity < 1.0f)
    {
        *pRequiresLayer = TRUE;

        goto Cleanup;
    }

    IFC(GetClippingRectangle(&ClipRect));

    if(!ClipRect.IsInfinite())
    {
        *pRequiresLayer = TRUE;

        goto Cleanup;
    }

    IFC(GetClippingGeometry(&pClipGeometry));

    if(pClipGeometry)
    {
        *pRequiresLayer = TRUE;

        goto Cleanup;
    }

Cleanup:
    ReleaseObject(pClipGeometry);

    return hr;
}

HRESULT CUIElement::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    BOOL NeedsLayer = FALSE;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    IFC(CVisual::PreRender(Context));

    IFC(RequiresLayer(&NeedsLayer));

    if(NeedsLayer)
    {
        if(m_Layer == NULL)
        {
            IFC(pRenderTarget->CreateLayer(&m_Layer));
        }
    }
    else
    {
        ReleaseObject(m_Layer);
    }

Cleanup:
    return hr;
}

BOOL CUIElement::ShouldClipToLayout()
{
    return m_ClipToLayoutBounds;
}

HRESULT CUIElement::GetClippingGeometry(CGraphicsGeometry** ppGeometry)
{
    HRESULT hr = S_OK;

    IFCPTR(ppGeometry);

    *ppGeometry = NULL;

Cleanup:
    return hr;
}

HRESULT CUIElement::GetClippingRectangle(RectF* pClippingRect)
{
    HRESULT hr = S_OK;

    IFCPTR(pClippingRect);

    if(ShouldClipToLayout())
    {
        *pClippingRect = MakeRect(m_FinalSize);
    }
    else
    {
        *pClippingRect = RectF::Infinite();
    }    

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CUIElement::GetLocalTransform(
    __out Matrix3X2F* pTransform
    )
{
    HRESULT hr = S_OK;

    *pTransform = Matrix3X2F::Translation(m_LayoutTransformOffset);

    return hr;
}

HRESULT CUIElement::Render(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    Visibility::Value EffectiveVisibility = Visibility::Visible;
    CRenderTarget* pRenderTarget = NULL;
    RectF ClipRect;
    CGraphicsGeometry* pClipGeometry = NULL;
    FLOAT Opacity = 1.0f;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    IFC(GetEffectiveVisibility(&EffectiveVisibility));

    if(EffectiveVisibility == Visibility::Visible)
    {
        if(m_Layer != NULL)
        {
            IFC(GetEffectiveOpacity(&Opacity));

            IFC(GetClippingRectangle(&ClipRect));

            IFC(GetClippingGeometry(&pClipGeometry));

            IFC(pRenderTarget->PushLayer(m_Layer, ClipRect, Opacity, pClipGeometry));
        }

        IFC(RenderInternal(Context));

        if(m_Layer != NULL)
        {
            IFC(pRenderTarget->PopLayer());
        }
    }

Cleanup:
    ReleaseObject(pClipGeometry);

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
    BOOL IsNamescope = FALSE;
    CNamescope* pNamescope = NULL;

    IFCEXPECT(!IsAttached());

    m_Attached = TRUE;

    IFC(GetEffectiveNamescope(&IsNamescope));

    if(IsNamescope)
    {
        IFC(CNamescope::Create(&pNamescope));

        m_Context = CUIAttachContext(Context.GetStaticTreeData(), Context.GetParent(), Context.GetTemplateParent(), pNamescope);
    }
    else
    {
        m_Context = Context;
    }

    m_NotifiedParentMeasureDirty = FALSE;
    m_NotifiedParentArrangeDirty = FALSE;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());

    IFC(CRoutedEventArgs::Create(&CUIElement::AttachedEvent, &pAttachedEventArgs));

    IFC(RaiseEvent(pAttachedEventArgs));
    
Cleanup:
    ReleaseObject(pNamescope);
    ReleaseObject(pAttachedEventArgs);

    return hr;
}

HRESULT CUIElement::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pDetachedEventArgs = NULL;

    IFCEXPECT(IsAttached());

    //TODO: Unfiy when dirtiness is actually required.
    IFC(InvalidateMeasure());

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

    return hr;
}

HRESULT CUIElement::GetMinMaxSize(SizeF& MinimumSize, SizeF& MaximumSize)
{
    HRESULT hr = S_OK;
    CFloatValue* pHeight = NULL;
    CFloatValue* pWidth = NULL;
    FLOAT MaxHeight = 0;
    FLOAT MinHeight = 0;
    FLOAT Height = 0;
    FLOAT Width = 0;
    FLOAT MaxWidth = 0;
    FLOAT MinWidth = 0;    

    IFC(m_Height.GetTypedEffectiveValue(&pHeight));

    IFC(GetEffectiveMaximumHeight(&MaxHeight));
    IFC(GetEffectiveMinimumHeight(&MinHeight));

    Height = (pHeight != NULL) ? pHeight->GetValue() : std::numeric_limits< FLOAT >::max();
    MaxHeight = std::max(std::min(Height, MaxHeight), MinHeight);

    Height = (pHeight != NULL) ? pHeight->GetValue() : 0;
    MinHeight = std::max(std::min(MaxHeight, Height), MinHeight);

    IFC(GetEffectiveMaximumWidth(&MaxWidth));
    IFC(GetEffectiveMinimumWidth(&MinWidth));

    IFC(m_Width.GetTypedEffectiveValue(&pWidth));

    Width = (pWidth != NULL) ? pWidth->GetValue() : std::numeric_limits< FLOAT >::max();
    MaxWidth = std::max(std::min(Width, MaxWidth), MinWidth);

    Width = (pWidth != NULL) ? pWidth->GetValue() : 0;
    MinWidth = std::max(std::min(MaxWidth, Width), MinWidth);

    MinimumSize.width = MinWidth;
    MinimumSize.height = MinHeight;

    MaximumSize.width = MaxWidth;
    MaximumSize.height = MaxHeight;

Cleanup:
    ReleaseObject(pWidth);
    ReleaseObject(pHeight);

    return hr;
}

HRESULT CUIElement::EnsureLayout()
{
    HRESULT hr = S_OK;
    CLayoutManager* pLayoutManager = NULL;

    pLayoutManager = GetLayoutManager();
    IFCPTR(pLayoutManager);

    IFC(pLayoutManager->EnsureLayout());    

Cleanup:
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
            RectF Margin;

            IFC(GetEffectiveMargin(&Margin));

            FLOAT MarginWidth = Margin.left + Margin.right;
            FLOAT MarginHeight = Margin.top + Margin.bottom;

            SizeF ElementAvailableSize(std::max(AvailableSize.width - MarginWidth, 0.0f), std::max(AvailableSize.height - MarginHeight, 0.0f));
            SizeF MinSize;
            SizeF MaxSize;

            IFC(GetMinMaxSize(MinSize, MaxSize));

            ElementAvailableSize.width = std::max(MinSize.width, std::min(ElementAvailableSize.width, MaxSize.width));
            ElementAvailableSize.height = std::max(MinSize.height, std::min(ElementAvailableSize.height, MaxSize.height));

            SizeF ElementDesiredSize;

            IFC(MeasureInternal(ElementAvailableSize, ElementDesiredSize));            

            ElementDesiredSize.width = std::max(ElementDesiredSize.width, MinSize.width);
            ElementDesiredSize.height = std::max(ElementDesiredSize.height, MinSize.height);

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

            FLOAT ClippedDesiredWidth = ElementDesiredSize.width + MarginWidth;
            FLOAT ClippedDesiredHeight = ElementDesiredSize.height + MarginHeight;

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

            m_DesiredSize.width = std::max(0.0f, ClippedDesiredWidth);
            m_DesiredSize.height = std::max(0.0f, ClippedDesiredHeight);

            IFC(InvalidateArrange());
        }
        else
        {
            m_DesiredSize.width = 0;
            m_DesiredSize.height = 0;
        }

        CleanMeasure();
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;

    DesiredSize.width = 0;
    DesiredSize.height = 0;

    return hr;
}

HRESULT CUIElement::GetEffectiveVisibility(Visibility::Value* pVisibility)
{
    HRESULT hr = S_OK;
    CVisibilityValue* pEffectiveValue = NULL;

    IFCPTR(pVisibility);

    IFC(m_Visibility.GetTypedEffectiveValue(&pEffectiveValue));

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

    IFC(m_MinimumWidth.GetTypedEffectiveValue(&pEffectiveValue));

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

    IFC(m_MinimumHeight.GetTypedEffectiveValue(&pEffectiveValue));

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

    IFC(m_MaximumWidth.GetTypedEffectiveValue(&pEffectiveValue));

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

    IFC(m_MaximumHeight.GetTypedEffectiveValue(&pEffectiveValue));

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

    IFC(m_HorizontalAlignment.GetTypedEffectiveValue(&pEffectiveValue));

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

    IFC(m_VerticalAlignment.GetTypedEffectiveValue(&pEffectiveValue));

    *pAlignment = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveMargin(RectF* pMargin)
{
    HRESULT hr = S_OK;
    CRectFValue* pEffectiveValue = NULL;

    IFC(m_Margin.GetTypedEffectiveValue(&pEffectiveValue));

    *pMargin = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveFocusable(BOOL* pFocusable)
{
    HRESULT hr = S_OK;
    CBoolValue* pEffectiveValue = NULL;

    IFC(m_Focusable.GetTypedEffectiveValue(&pEffectiveValue));

    *pFocusable = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveOpacity(FLOAT* pOpacity)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFCPTR(pOpacity);

    IFC(m_Opacity.GetTypedEffectiveValue(&pEffectiveValue));

    *pOpacity = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::GetEffectiveNamescope(BOOL* pNamescope)
{
    HRESULT hr = S_OK;
    CBoolValue* pEffectiveValue = NULL;

    IFC(m_Namescope.GetTypedEffectiveValue(&pEffectiveValue));

    *pNamescope = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CUIElement::SetIsNamescope(BOOL IsNamescope)
{
    HRESULT hr = S_OK;
    CBoolValue* pValue = NULL;

    IFC(CBoolValue::Create(IsNamescope, &pValue));

    IFC(SetValue(&CUIElement::NamescopeProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

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

            SizeF ArrangeSize(Bounds.right - Bounds.left, Bounds.bottom - Bounds.top);
            SizeF OriginalArrangeSize = ArrangeSize;

            RectF Margin;
            
            IFC(GetEffectiveMargin(&Margin));

            FLOAT MarginWidth = Margin.left + Margin.right;
            FLOAT MarginHeight = Margin.top + Margin.bottom;

            ArrangeSize.width = std::max(0.0f, ArrangeSize.width - MarginWidth);
            ArrangeSize.height = std::max(0.0f, ArrangeSize.height - MarginHeight);

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

            SizeF MinSize;
            SizeF MaxSize;

            IFC(GetMinMaxSize(MinSize, MaxSize));      

            FLOAT EffectiveMaxWidth = std::max(m_UnclippedDesiredSize.width, MaxSize.width);

            if(EffectiveMaxWidth < ArrangeSize.width)
            {
                NeedsClipBounds = TRUE;
                ArrangeSize.width = EffectiveMaxWidth;
            }

            FLOAT EffectiveMaxHeight = std::max(m_UnclippedDesiredSize.height, MaxSize.height);

            if(EffectiveMaxHeight < ArrangeSize.height)
            {
                NeedsClipBounds = TRUE;
                ArrangeSize.height = EffectiveMaxHeight;
            }

            SizeF UsedArrangeSize;

            IFC(ArrangeInternal(ArrangeSize, UsedArrangeSize));

            m_FinalSize = UsedArrangeSize;

            SizeF ClippedSize(std::min(UsedArrangeSize.width, MaxSize.width), std::min(UsedArrangeSize.height, MaxSize.height));

            NeedsClipBounds |= (ClippedSize.width < UsedArrangeSize.width) ||  (ClippedSize.height < UsedArrangeSize.height);

            SizeF ClientSize(std::max(0.0f, OriginalArrangeSize.width - MarginWidth), std::max(0.0f, OriginalArrangeSize.height - MarginHeight));

            NeedsClipBounds |= (ClientSize.width < ClippedSize.width) || (ClientSize.height < ClippedSize.height);

            SizeF Offset;

            IFC(ComputeAlignmentOffset(ClientSize, ClippedSize, Offset));

            Offset.width += Bounds.left + Margin.left;
            Offset.height += Bounds.top + Margin.top;

            Matrix3X2F VisualTransform = Matrix3X2F::Translation(Offset);

            m_LayoutTransformOffset = Offset;

            m_ClipToLayoutBounds = NeedsClipBounds;
        }
        else
        {
            m_FinalSize.width = 0;
            m_FinalSize.height = 0;
        }

        CleanArrange();
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::ComputeAlignmentOffset(SizeF ClientSize, SizeF RenderSize, SizeF& Offset)
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

    if(HorzAlign == HorizontalAlignment::Center || HorzAlign == HorizontalAlignment::Stretch)
    {
        Offset.width = (ClientSize.width - RenderSize.width) * 0.5f;
    }
    else if(HorzAlign == HorizontalAlignment::Right)
    {
        Offset.width = ClientSize.width - RenderSize.width;
    }
    else
    {
        Offset.width = 0;
    }

    if(VertAlign == VerticalAlignment::Center || VertAlign == VerticalAlignment::Stretch)
    {
        Offset.height = (ClientSize.height - RenderSize.height) * 0.5f;
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

    UsedSize = AvailableSize;

    return hr;
}

HRESULT CUIElement::InvalidateMeasure()
{
    HRESULT hr = S_OK;
    CChildMeasureInvalidatedNotification* pNotification = NULL;

    if(!m_MeasureDirty)
    {
        m_MeasureDirty = TRUE;
    }

    if(!m_NotifiedParentMeasureDirty && GetParent() != NULL)
    {
        m_NotifiedParentMeasureDirty = TRUE;

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
    }

    if(!m_NotifiedParentArrangeDirty && GetParent() != NULL)
    {
        m_NotifiedParentArrangeDirty = TRUE;

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

void CUIElement::CleanMeasure()
{
    m_MeasureDirty = FALSE;
    m_NotifiedParentMeasureDirty = FALSE;
}

void CUIElement::CleanArrange()
{
    m_ArrangeDirty = FALSE;
    m_NotifiedParentArrangeDirty = FALSE;
}

CUIElement* CUIElement::GetParent()
{
    return m_Context.GetParent();
}

CUIElement* CUIElement::GetTemplateParent()
{
    return m_Context.GetTemplateParent();
}

CFocusManager* CUIElement::GetFocusManager()
{
    return m_Context.GetFocusManager();
}

CNamescope* CUIElement::GetNamescope()
{
    return m_Context.GetNamescope();
}

CMouseController* CUIElement::GetMouseController()
{
    return m_Context.GetMouseController();
}

CKeyboardController* CUIElement::GetKeyboardController()
{
    return m_Context.GetKeyboardController();
}

CStaticTreeData* CUIElement::GetStaticTreeData()
{
    return m_Context.GetStaticTreeData();
}

CProviders* CUIElement::GetProviders()
{
    return m_Providers;
}

CTypeConverter* CUIElement::GetTypeConverter()
{
    return GetProviders()->GetTypeConverter();
}

CBindingManager* CUIElement::GetBindingManager()
{
    return GetProviders()->GetBindingManager();
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
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

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
        &MarginProperty,
        &FocusableProperty,
        &OpacityProperty,
        &NamescopeProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CVisual::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CUIElement::CreateEventInformation(CEventInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CRoutedEventInformation* pEventInformation = NULL;

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
        &MouseMoveEvent,
        &MouseEnterEvent,
        &MouseLeaveEvent,
        &PreviewGotFocusEvent,
        &GotFocusEvent,
        &PreviewLostFocusEvent,
        &LostFocusEvent,
        &KeyEvent,
        &KeyDownEvent,
        &KeyUpEvent,
        &TextEvent
    };
    
    IFCPTR(ppInformation);

    IFC(CRoutedEventInformation::Create(Events, ARRAYSIZE(Events), &pEventInformation));

    *ppInformation = pEventInformation;
    pEventInformation = NULL;

Cleanup:
    ReleaseObject(pEventInformation);

    return hr;
}

HRESULT CUIElement::CreateCommandInformation(CCommandInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticCommandInformation* pCommandInformation = NULL;
    
    IFCPTR(ppInformation);

    IFC(CStaticCommandInformation::Create(NULL, NULL, &pCommandInformation));

    *ppInformation = pCommandInformation;
    pCommandInformation = NULL;

Cleanup:
    ReleaseObject(pCommandInformation);

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
        *ppLayeredValue = &m_MaximumWidth;
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
    else if(pProperty == &CUIElement::MarginProperty)
    {
        *ppLayeredValue = &m_Margin;
    }
    else if(pProperty == &CUIElement::FocusableProperty)
    {
        *ppLayeredValue = &m_Focusable;
    }
    else if(pProperty == &CUIElement::OpacityProperty)
    {
        *ppLayeredValue = &m_Opacity;
    }
    else if(pProperty == &CUIElement::NamescopeProperty)
    {
        *ppLayeredValue = &m_Namescope;
    }
    else
    {
        hr = E_FAIL;
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::ClearValue(CProperty* pProperty)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);

    IFC(GetLayeredValue(pProperty, &pLayeredValue));

    IFC(pLayeredValue->ClearLocalValue());

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

HRESULT CUIElement::OnMarginChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnFocusableChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CUIElement::OnOpacityChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    //TODO: Invalidate render.

    return hr;
}

BOOL CUIElement::IsFocusable()
{
    BOOL Focusable = FALSE;
    BOOL EffectiveFocusable = FALSE;
    Visibility::Value EffectiveVisibility = Visibility::Visible;

    if(SUCCEEDED(GetEffectiveFocusable(&EffectiveFocusable)) && SUCCEEDED(GetEffectiveVisibility(&EffectiveVisibility)))
    {
        Focusable = EffectiveFocusable && (EffectiveVisibility == Visibility::Visible);
    }

    return Focusable;
}

HRESULT CUIElement::Focus(BOOL* pSetFocus)
{
    HRESULT hr = S_OK;
    CFocusManager* pFocusManager = NULL;

    pFocusManager = GetFocusManager();
    IFCPTR(pFocusManager);

    IFC(pFocusManager->SetFocus(this, pSetFocus));

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

        case RoutingStrategy::Tunneled:
            {
                IFC(InternalRaiseTunneledEvent(pRoutedEventArgs));

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

HRESULT CUIElement::InternalRaiseTunneledEvent(CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CUIElement* pParent = NULL;

    IFCPTR(pRoutedEventArgs);

    pParent = GetParent();

    if(pParent)
    {
        IFC(pParent->InternalRaiseTunneledEvent(pRoutedEventArgs));
    }

    if(!pRoutedEventArgs->IsHandled())
    {
        IFC(InternalRaiseEvent(pRoutedEventArgs));
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::AddHandler(CRoutedEvent* pRoutedEvent, const RoutedEventHandler& Handler, events::signals::connection* pConnection)
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
}

void CUIElement::OnMouseLeftButtonDown(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CMouseButtonEventArgs* pMouseButtonEventArgs = NULL;
    BOOL SetFocus = FALSE;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFCEXPECT(pRoutedEventArgs->IsTypeOf(TypeIndex::MouseButtonEventArgs));

    pMouseButtonEventArgs = (CMouseButtonEventArgs*)pRoutedEventArgs;

    // Set focus to the element if it's focusable.
    IFC(Focus(&SetFocus));

    if(SetFocus)
    {
        pMouseButtonEventArgs->SetHandled(TRUE);
    }

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

void CUIElement::OnKey(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CKeyEventArgs* pKeyEventArgs = NULL;
    CKeyEventArgs* pNewEventArgs = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFCEXPECT(pRoutedEventArgs->IsTypeOf(TypeIndex::KeyEventArgs));

    pKeyEventArgs = (CKeyEventArgs*)pRoutedEventArgs;

    if(!pKeyEventArgs->IsHandled())
    {
        if(pKeyEventArgs->GetState() == KeyState::Down)
        {
            IFC(CKeyEventArgs::Create(&KeyDownEvent, pKeyEventArgs, &pNewEventArgs));

            IFC(RaiseEvent(pNewEventArgs));

            if(pNewEventArgs->IsHandled())
            {
                pKeyEventArgs->SetHandled();
            }
        }
        else
        {
            IFC(CKeyEventArgs::Create(&KeyUpEvent, pKeyEventArgs, &pNewEventArgs));

            IFC(RaiseEvent(pNewEventArgs));

            if(pNewEventArgs->IsHandled())
            {
                pKeyEventArgs->SetHandled();
            }
        }
    }

Cleanup:
    ReleaseObject(pNewEventArgs);
}

HRESULT CUIElement::SetLocalBindingValue(CPropertyObject* pTarget, CProperty* pTargetProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CUIElement* pUIElement = NULL;

    IFC(CastType(pTarget, &pUIElement));

    IFC(pUIElement->SetValue(pTargetProperty, pValue));

Cleanup:
    return hr;
}

HRESULT CUIElement::SetBinding(CProperty* pProperty, CBindingBase* pBinding)
{
    HRESULT hr = S_OK;

    IFC(GetBindingManager()->SetBinding(this, pProperty, pBinding, &CUIElement::SetLocalBindingValue));

Cleanup:
    return hr;
}

HRESULT CUIElement::CaptureMouse()
{
    HRESULT hr = S_OK;
    CMouseController* pMouseController = NULL;

    pMouseController = GetMouseController();
    IFCPTR(pMouseController);

    IFC(pMouseController->SetCapture(this));

Cleanup:
    return hr;
}

HRESULT CUIElement::ReleaseMouse()
{
    HRESULT hr = S_OK;
    CMouseController* pMouseController = NULL;

    pMouseController = GetMouseController();
    IFCPTR(pMouseController);

    IFC(pMouseController->ReleaseCapture(this));

Cleanup:
    return hr;
}

CTimeSource* CUIElement::GetTimeSource()
{
    return m_Context.GetTimeSource();
}

CLayoutManager* CUIElement::GetLayoutManager()
{
    return m_Context.GetLayoutManager();
}

HRESULT CUIElement::GetAnimationBaseValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    IFC(GetEffectiveValue(pProperty, ppValue));

Cleanup:
    return hr;
}

HRESULT CUIElement::SetAnimationValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFC(GetLayeredValue(pProperty, &pLayeredValue));

    IFC(pLayeredValue->SetAnimationValue(pValue));

Cleanup:
    return hr;
}

HRESULT CUIElement::IsHitTestVisible(BOOL* pVisible)
{
    HRESULT hr = S_OK;
    Visibility::Value EffectiveVisibility;
    BOOL HitTestVisible = TRUE;

    IFC(GetEffectiveVisibility(&EffectiveVisibility));

    if(HitTestVisible)
    {
        HitTestVisible = (EffectiveVisibility == Visibility::Visible);
    }

    *pVisible = HitTestVisible;

Cleanup:
    return hr;
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

HRESULT CEventHandlerChain::AddHandler(const RoutedEventHandler& Handler, events::signals::connection* pConnection)
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

//
// CUIElement
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CUIElement_TypeIndex()
{
    return TypeIndex::UIElement;
}

extern "C" UIFRAMEWORK_API
CVisual* CUIElement_CastTo_CVisual(CUIElement* pElement)
{
    return pElement;
}

extern "C" UIFRAMEWORK_API
CUIElement* CObjectWithType_CastTo_CUIElement(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::UIElement)) ? (CUIElement*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
CNamescope* CUIElement_GetNamescope(CUIElement* pElement)
{
    return pElement->GetNamescope();
}

typedef void (*HandlerFunc)( CObjectWithType*, CRoutedEventArgs* );

extern "C" UIFRAMEWORK_API
HRESULT CUIElement_AddHandler(CUIElement* pElement, CRoutedEvent* pRoutedEvent, HandlerFunc Handler, events::signals::connection** ppConnection)
{
    HRESULT hr = S_OK;
    events::signals::connection Con;

    IFCPTR(ppConnection);

    IFC(pElement->AddHandler(pRoutedEvent, Handler, &Con));

    *ppConnection = new events::signals::connection(Con);
    IFCOOM(ppConnection);

Cleanup:
    return hr;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetAttachedEvent()
{
    return &CUIElement::AttachedEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetDetachedEvent()
{
    return &CUIElement::DetachedEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseButtonEvent()
{
    return &CUIElement::MouseButtonEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseDownEvent()
{
    return &CUIElement::MouseDownEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseUpEvent()
{
    return &CUIElement::MouseUpEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseLeftButtonDownEvent()
{
    return &CUIElement::MouseLeftButtonDownEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseRightButtonDownEvent()
{
    return &CUIElement::MouseRightButtonDownEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseMiddleButtonDownEvent()
{
    return &CUIElement::MouseMiddleButtonDownEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseLeftButtonUpEvent()
{
    return &CUIElement::MouseLeftButtonUpEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseRightButtonUpEvent()
{
    return &CUIElement::MouseRightButtonUpEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseMiddleButtonUpEvent()
{
    return &CUIElement::MouseMiddleButtonUpEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseMoveEvent()
{
    return &CUIElement::MouseMoveEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseEnterEvent()
{
    return &CUIElement::MouseEnterEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetMouseLeaveEvent()
{
    return &CUIElement::MouseLeaveEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetPreviewGotFocusEvent()
{
    return &CUIElement::PreviewGotFocusEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetGotFocusEvent()
{
    return &CUIElement::GotFocusEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetPreviewLostFocusEvent()
{
    return &CUIElement::PreviewLostFocusEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetLostFocusEvent()
{
    return &CUIElement::LostFocusEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetKeyEvent()
{
    return &CUIElement::KeyEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetKeyDownEvent()
{
    return &CUIElement::KeyDownEvent;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CUIElement_GetKeyUpEvent()
{
    return &CUIElement::KeyUpEvent;
}

extern "C" UIFRAMEWORK_API
CProperty* CUIElement_GetVisibilityProperty()
{
    return &CUIElement::VisibilityProperty;
}

extern "C" UIFRAMEWORK_API
HRESULT CUIElement_Focus(CUIElement* pElement, BOOL* pSetFocus)
{
    return pElement->Focus(pSetFocus);
}

extern "C" UIFRAMEWORK_API
HRESULT CUIElement_EnsureLayout(CUIElement* pElement)
{
    return pElement->EnsureLayout();
}

//
// Connection
//
extern "C" UIFRAMEWORK_API
void Connection_DisconnectAndDelete(events::signals::connection* pConnection)
{
    pConnection->disconnect();

    delete pConnection;
}