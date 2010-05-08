#include "UIElement.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "DelegatingPropertyInformation.h"
#include "MouseInput.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Width, CFloatValue, 0 );
DEFINE_GET_DEFAULT( Height, CFloatValue, 0 );
DEFINE_GET_DEFAULT( MinimumWidth, CFloatValue, 0 );
DEFINE_GET_DEFAULT( MinimumHeight, CFloatValue, 0 );
DEFINE_GET_DEFAULT( MaximumWidth, CFloatValue, FLT_MAX );
DEFINE_GET_DEFAULT( MaximumHeight, CFloatValue, FLT_MAX );
DEFINE_GET_DEFAULT( Visibility, CVisibilityValue, Visibility::Visible );

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

//
// Events
//
CStaticRoutedEvent< RoutingStrategy::Bubbling > CUIElement::MouseButtonEvent;

CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseDownEvent;
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseUpEvent;

CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseLeftButtonDownEvent;
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseRightButtonDownEvent;
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseMiddleButtonDownEvent;

CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseLeftButtonUpEvent;
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseRightButtonUpEvent;
CStaticRoutedEvent< RoutingStrategy::Direct > CUIElement::MouseMiddleButtonUpEvent;

CStaticRoutedEvent< RoutingStrategy::Bubbling > CUIElement::MouseMoveEvent;

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
                           m_Visibility(this, &CUIElement::VisibilityProperty)
{
    m_DesiredSize.width = 0;
    m_DesiredSize.height = 0;

    m_LastMeasureSize.width = 0;
    m_LastMeasureSize.height = 0;

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
}

HRESULT CUIElement::Initialize()
{
    HRESULT hr = S_OK;

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

    IFCEXPECT(!IsAttached());

    m_Attached = TRUE;
    m_Context = Context;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());
    
Cleanup:
    return hr;
}

HRESULT CUIElement::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFCEXPECT(IsAttached());

    m_Attached = FALSE;

    m_Context.Reset();
    
Cleanup:
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

HRESULT CUIElement::Measure(SizeF AvailableSize)
{
    HRESULT hr = S_OK;

    if(m_MeasureDirty || m_LastMeasureSize.height != AvailableSize.height || m_LastMeasureSize.width != AvailableSize.width)
    {
        Visibility::Value EffectiveVisibility = Visibility::Visible;

        IFC(GetEffectiveVisibility(&EffectiveVisibility));

        if(EffectiveVisibility == Visibility::Visible || EffectiveVisibility == Visibility::Hidden)
        {
            SizeF NewSize = { 0 };
            FLOAT MinimumWidth = 0;
            FLOAT MinimumHeight = 0;
            FLOAT MaximumWidth = 0;
            FLOAT MaximumHeight = 0;

            IFC(GetEffectiveMinimumWidth(&MinimumWidth));
            IFC(GetEffectiveMinimumHeight(&MinimumHeight));
            IFC(GetEffectiveMaximumWidth(&MaximumWidth));
            IFC(GetEffectiveMaximumHeight(&MaximumHeight));

            IFC(MeasureInternal(AvailableSize, NewSize));

            m_LastMeasureSize = AvailableSize;

            NewSize.width = max(NewSize.width, MinimumWidth);
            NewSize.height = max(NewSize.height, MinimumHeight);

            NewSize.width = min(NewSize.width, MaximumWidth);
            NewSize.height = min(NewSize.height, MaximumHeight);

            if(NewSize.width != m_DesiredSize.width || NewSize.height != m_DesiredSize.width)
            {
                m_DesiredSize = NewSize;

                IFC(InvalidateArrange());
            }
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
    FLOAT Width = 0;
    FLOAT Height = 0;

    IFCEXPECT(AvailableSize.width >= 0);
    IFCEXPECT(AvailableSize.height >= 0);

    IFC(GetEffectiveWidth(&Width));
    IFC(GetEffectiveHeight(&Height));

    DesiredSize.width = min(AvailableSize.width, Width);
    DesiredSize.height = min(AvailableSize.height, Height);

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

SizeF CUIElement::GetDesiredSize()
{
    return m_DesiredSize;
}

SizeF CUIElement::GetFinalSize()
{
    return m_FinalSize;
}

HRESULT CUIElement::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Size.width >= 0);
    IFCEXPECT(Size.height >= 0);

    if(m_ArrangeDirty || Size.width != m_FinalSize.width || Size.height != m_FinalSize.width)
    {
        Visibility::Value EffectiveVisibility = Visibility::Visible;

        IFC(GetEffectiveVisibility(&EffectiveVisibility))

        if(EffectiveVisibility == Visibility::Visible || EffectiveVisibility == Visibility::Hidden)
        {
            m_FinalSize = Size;

            IFC(ArrangeInternal(Size));
        }
        else
        {
            m_FinalSize.width = 0;
            m_FinalSize.height = 0;
        }

        m_ArrangeDirty = FALSE;
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;

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

CProviders* CUIElement::GetProviders()
{
    return m_Context.GetProviders();
}

CTypeConverter* CUIElement::GetTypeConverter()
{
    return (m_Context.GetProviders() != NULL) ? m_Context.GetProviders()->GetTypeConverter() : NULL;
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
        &VisibilityProperty
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
    else
    {
        hr = E_FAIL;
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::SetValue(CProperty* pProperty, CObjectWithType* pValue)
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
            IFC(SetValueInternal(pProperty, pValue));
        }
    }
    else
    {
        if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)))
        {
            IFC(pLayeredValue->SetLocalValue(pValue, GetProviders()));
        }
        else
        {
            IFC(SetValueInternal(pProperty, pValue));
        }
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(CVisual::SetValue(pProperty, pValue));

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
        if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)))
        {
            IFC(pLayeredValue->GetLocalValue(GetProviders(), ppValue));
        }
        else
        {
            IFC(GetValueInternal(pProperty, ppValue));
        }
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