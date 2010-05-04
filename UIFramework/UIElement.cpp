#include "UIElement.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "DelegatingPropertyInformation.h"
#include "MouseInput.h"

CStaticProperty UIElementProperties[] = 
{
    CStaticProperty( L"Width", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"Height", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"MinimumWidth", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"MinimumHeight", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"MaximumWidth", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"MaximumHeight", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"Visibility", TypeIndex::Visibility, StaticPropertyFlags::None )
};

namespace UIElementPropertyIndex
{
    enum Value
    {
        Width,
        Height,
        MinimumWidth,
        MinimumHeight,
        MaximumWidth,
        MaximumHeight,
        Visibility
    };
}

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
                           m_Visibility(Visibility::Visible)
{
    m_Size.width = 0;
    m_Size.height = 0;

    m_MinimumSize.width = 0;
    m_MinimumSize.height = 0;

    m_MaximumSize.width = FLT_MAX;
    m_MaximumSize.height = FLT_MAX;

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

    if(m_Visibility == Visibility::Visible)
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

    if(m_Visibility == Visibility::Visible)
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

    IFC(InternalSetWidth(Size.width));
    IFC(InternalSetHeight(Size.height));

Cleanup:
    return hr;
}

HRESULT CUIElement::Measure(SizeF AvailableSize)
{
    HRESULT hr = S_OK;

    if(m_MeasureDirty || m_LastMeasureSize.height != AvailableSize.height || m_LastMeasureSize.width != AvailableSize.width)
    {
        if(m_Visibility == Visibility::Visible || m_Visibility == Visibility::Hidden)
        {
            SizeF NewSize = { 0 };

            IFC(MeasureInternal(AvailableSize, NewSize));

            m_LastMeasureSize = AvailableSize;

            NewSize.width = max(NewSize.width, m_MinimumSize.width);
            NewSize.height = max(NewSize.height, m_MinimumSize.height);

            NewSize.width = min(NewSize.width, m_MaximumSize.width);
            NewSize.height = min(NewSize.height, m_MaximumSize.height);

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

    IFCEXPECT(AvailableSize.width >= 0);
    IFCEXPECT(AvailableSize.height >= 0);

    DesiredSize.width = min(AvailableSize.width, m_Size.width);
    DesiredSize.height = min(AvailableSize.height, m_Size.height);

Cleanup:
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
        if(m_Visibility == Visibility::Visible || m_Visibility == Visibility::Hidden)
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

    IFC(InternalSetVisibility(State));

Cleanup:
    return hr;
}

HRESULT CUIElement::InternalSetVisibility(Visibility::Value State)
{
    HRESULT hr = S_OK;

    IFCEXPECT(State == Visibility::Visible || State == Visibility::Hidden || State == Visibility::Collapsed);

    m_Visibility = State;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());

Cleanup:
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

    IFC(CStaticPropertyInformation::Create(UIElementProperties, ARRAYSIZE(UIElementProperties), &pStaticInformation))
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

HRESULT CUIElement::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= UIElementProperties && pProperty < UIElementProperties + ARRAYSIZE(UIElementProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - UIElementProperties);
        
        switch(Index)
        {
            case UIElementPropertyIndex::Width:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(InternalSetHeight(pFloat->GetValue()));

                    break;
                }

            case UIElementPropertyIndex::Height:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(InternalSetWidth(pFloat->GetValue()));

                    break;
                }

            case UIElementPropertyIndex::MinimumWidth:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(InternalSetMinimumHeight(pFloat->GetValue()));

                    break;
                }

            case UIElementPropertyIndex::MinimumHeight:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(InternalSetMinimumWidth(pFloat->GetValue()));

                    break;
                }

            case UIElementPropertyIndex::MaximumWidth:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(InternalSetMaximumHeight(pFloat->GetValue()));

                    break;
                }

            case UIElementPropertyIndex::MaximumHeight:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(InternalSetMaximumWidth(pFloat->GetValue()));

                    break;
                }

            case UIElementPropertyIndex::Visibility:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Visibility));

                    CVisibilityValue* pVisibility = (CVisibilityValue*)pValue;

                    IFC(InternalSetVisibility(pVisibility->GetValue()));

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CVisual::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CFloatValue* pFloatValue = NULL;
    CVisibilityValue* pVisibilityValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    // Check if the property is a static property.
    if(pProperty >= UIElementProperties && pProperty < UIElementProperties + ARRAYSIZE(UIElementProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - UIElementProperties);
        
        switch(Index)
        {
            case UIElementPropertyIndex::Width:
                {
                    IFC(CFloatValue::Create(m_Size.width, &pFloatValue));

                    *ppValue = pFloatValue;
                    pFloatValue = NULL;

                    break;
                }

            case UIElementPropertyIndex::Height:
                {
                    IFC(CFloatValue::Create(m_Size.width, &pFloatValue));

                    *ppValue = pFloatValue;
                    pFloatValue = NULL;

                    break;
                }

            case UIElementPropertyIndex::Visibility:
                {
                    IFC(CVisibilityValue::Create(m_Visibility, &pVisibilityValue));

                    *ppValue = pVisibilityValue;
                    pVisibilityValue = NULL;

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CVisual::GetValue(pProperty, ppValue));
    }

Cleanup:
    ReleaseObject(pFloatValue);
    ReleaseObject(pVisibilityValue);

    return hr;
}

HRESULT CUIElement::InternalSetWidth(FLOAT Width)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Width >= 0);

    m_Size.width = Width;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}
HRESULT CUIElement::InternalSetHeight(FLOAT Height)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Height >= 0);

    m_Size.height = Height;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::InternalSetMinimumWidth(FLOAT Width)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Width >= 0);

    m_MinimumSize.width = Width;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}
HRESULT CUIElement::InternalSetMinimumHeight(FLOAT Height)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Height >= 0);

    m_MinimumSize.height = Height;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::InternalSetMaximumWidth(FLOAT Width)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Width >= 0);

    m_MaximumSize.width = Width;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}
HRESULT CUIElement::InternalSetMaximumHeight(FLOAT Height)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Height >= 0);

    m_MaximumSize.height = Height;

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