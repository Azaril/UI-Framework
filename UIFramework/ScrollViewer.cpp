#include "ScrollViewer.h"
#include "RoutedCommand.h"
#include "RoutedCommandEventArgs.h"
#include "ScrollBar.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
namespace ScrollViewerProperties
{
    enum Value
    {
        ExtentHeight,
        ExtentWidth,
        ViewportWidth,
        ViewportHeight,
        HorizontalOffset,
        VerticalOffset,
        ScrollableWidth,
        ScrollableHeight,
        ComputedHorizontalScrollBarVisibility,
        ComputedVerticalScrollBarVisibility
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT( ExtentHeight, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( ExtentWidth, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( ViewportWidth, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( ViewportHeight, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( HorizontalOffset, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( VerticalOffset, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( ScrollableWidth, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( ScrollableHeight, FLOAT, 0.0f );
DEFINE_GET_DEFAULT( ComputedVerticalScrollBarVisibility, Visibility::Value, Visibility::Collapsed );
DEFINE_GET_DEFAULT( ComputedHorizontalScrollBarVisibility, Visibility::Value, Visibility::Collapsed );

//
// Properties
//
CStaticProperty CScrollViewer::ExtentHeightProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::ExtentHeight, L"ExtentHeight", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ExtentHeight ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnExtentHeightChanged ) );
CStaticProperty CScrollViewer::ExtentWidthProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::ExtentWidth, L"ExtentWidth", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ExtentWidth ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnExtentWidthChanged ) );
CStaticProperty CScrollViewer::ViewportWidthProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::ViewportWidth, L"ViewportWidth", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ViewportWidth ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnViewportWidthChanged ) );
CStaticProperty CScrollViewer::ViewportHeightProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::ViewportHeight, L"ViewportHeight", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ViewportHeight ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnViewportHeightChanged ) );
CStaticProperty CScrollViewer::HorizontalOffsetProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::HorizontalOffset, L"HorizontalOffset", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( HorizontalOffset ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnHorizontalOffsetChanged ) );
CStaticProperty CScrollViewer::VerticalOffsetProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::VerticalOffset, L"VerticalOffset", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( VerticalOffset ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnVerticalOffsetChanged ) );
CStaticProperty CScrollViewer::ScrollableWidthProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::ScrollableWidth, L"ScrollableWidth", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ScrollableWidth ) );
CStaticProperty CScrollViewer::ScrollableHeightProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::ScrollableHeight, L"ScrollableHeight", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ScrollableHeight ) );
CStaticProperty CScrollViewer::ComputedHorizontalScrollBarVisibilityProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::ComputedHorizontalScrollBarVisibility, L"ComputedHorizontalScrollBarVisibility", TypeIndex::Visibility, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ComputedVerticalScrollBarVisibility ) );
CStaticProperty CScrollViewer::ComputedVerticalScrollBarVisibilityProperty(TypeIndex::ScrollViewer, ScrollViewerProperties::ComputedVerticalScrollBarVisibility, L"ComputedVerticalScrollBarVisibility", TypeIndex::Visibility, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ComputedHorizontalScrollBarVisibility ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnExtentHeightChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnExtentWidthChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnViewportWidthChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnViewportHeightChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnHorizontalOffsetChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnVerticalOffsetChanged );

CScrollViewer::CScrollViewer() : m_ScrollPresenter(NULL),
                                 m_InMeasure(FALSE)
{
}

CScrollViewer::~CScrollViewer()
{
    m_RoutedCommandExecutedConnection.disconnect();

    ReleaseObject(m_ScrollPresenter);
}

HRESULT CScrollViewer::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CContentControl::Initialize(pProviders));

    IFC(AddHandler(&CRoutedCommand::ExecutedEvent, boost::bind(&CScrollViewer::OnCommandExecuted, this, _1, _2), &m_RoutedCommandExecutedConnection));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CScrollViewer::MeasureInternal( 
    const SizeF& AvailableSize,
    SizeF& DesiredSize
    )
{
    HRESULT hr = S_OK;
    CUIElement* pChild = NULL;
    ScrollBarVisibility::Value ScrollVerticalVisibility = ScrollBarVisibility::Auto;
    ScrollBarVisibility::Value ScrollHorizontalVisibility = ScrollBarVisibility::Auto;
    SizeF ElementDesiredSize;

    //TODO: Add scroll bar visibility properties.

    //IFC(GetBasicTypeEffectiveValue(&ScrollProp&ScrollVerticalVisibility));
    //IFC(GetBasicTypeEffectiveValue(&ScrollHorizontalVisibility));

    {
        bool AutoVerticalVisibility = (ScrollVerticalVisibility == ScrollBarVisibility::Auto);
        bool AutoHorizontalVisibility = (ScrollHorizontalVisibility == ScrollBarVisibility::Auto);
        bool VerticalDisableScrollBar = (ScrollVerticalVisibility == ScrollBarVisibility::Disabled);
        bool HorizontalDisableScrollBar = (ScrollHorizontalVisibility == ScrollBarVisibility::Disabled);
        Visibility::Value VerticalVisibility = (ScrollVerticalVisibility == ScrollBarVisibility::Visible) ? Visibility::Visible : Visibility::Collapsed;
        Visibility::Value HorizontalVisibility = (ScrollHorizontalVisibility == ScrollBarVisibility::Visible) ? Visibility::Visible : Visibility::Collapsed;

        m_InMeasure = TRUE;

        IFC(SetComputedVerticalScrollBarVisibility(VerticalVisibility));
        IFC(SetComputedHorizontalScrollBarVisibility(HorizontalVisibility));

        if(m_ScrollPresenter != NULL)
        {
            IFC(m_ScrollPresenter->SetCanScrollVertically(!VerticalDisableScrollBar));
            IFC(m_ScrollPresenter->SetCanScrollHorizontally(!HorizontalDisableScrollBar));
        }

        IFC(GetTemplateChild(&pChild));

        if(pChild != NULL)
        {
            IFC(pChild->Measure(AvailableSize));
        }

        if (pChild != NULL && m_ScrollPresenter != NULL && (AutoVerticalVisibility || AutoHorizontalVisibility))
        {
            SizeF CurrentExtent = m_ScrollPresenter->GetExtent();
            SizeF CurrentViewport = m_ScrollPresenter->GetViewport();

            bool MakeHorizontalBarVisible = AutoHorizontalVisibility && (CurrentExtent.width > CurrentViewport.width);
            bool MakeVerticalBarVisible = AutoVerticalVisibility && (CurrentExtent.height > CurrentViewport.height);

            if(MakeVerticalBarVisible)
            {
                IFC(SetComputedVerticalScrollBarVisibility(Visibility::Visible));
            }

            if(MakeHorizontalBarVisible)
            {
                IFC(SetComputedHorizontalScrollBarVisibility(Visibility::Visible));
            }

            if(MakeVerticalBarVisible || MakeHorizontalBarVisible)
            {
                IFC(pChild->InvalidateMeasure());

                IFC(pChild->Measure(AvailableSize));
            }

            if(MakeVerticalBarVisible && MakeHorizontalBarVisible && (MakeVerticalBarVisible != MakeHorizontalBarVisible))
            {
                SizeF SecondPassCurrentExtent = m_ScrollPresenter->GetExtent();
                SizeF SecondPassCurrentViewport = m_ScrollPresenter->GetViewport();

                bool SecondPassMakeHorizontalBarVisible = !MakeHorizontalBarVisible && (SecondPassCurrentExtent.width > SecondPassCurrentViewport.width);
                bool SecondPassMakeVerticalBarVisible = !MakeVerticalBarVisible && (SecondPassCurrentExtent.height > SecondPassCurrentViewport.height);

                if(SecondPassMakeHorizontalBarVisible)
                {
                    IFC(SetComputedHorizontalScrollBarVisibility(Visibility::Visible));
                }
                else if(SecondPassMakeVerticalBarVisible)
                {
                    IFC(SetComputedVerticalScrollBarVisibility(Visibility::Visible));
                }

                if(SecondPassMakeHorizontalBarVisible || SecondPassMakeVerticalBarVisible)
                {
                    IFC(pChild->InvalidateMeasure());

                    IFC(pChild->Measure(AvailableSize));
                }
            }
        }

        if(pChild != NULL)
        {
            ElementDesiredSize = pChild->GetDesiredSize();
        }
    }

    DesiredSize = ElementDesiredSize;

Cleanup:
    ReleaseObject(pChild);

    m_InMeasure = FALSE;

    return hr;
}

HRESULT CScrollViewer::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CControl::OnAttach(Context));

Cleanup:
    return hr;
}

HRESULT CScrollViewer::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CControl::OnDetach(Context));

Cleanup:
    return hr;
}

HRESULT CScrollViewer::SetComputedVerticalScrollBarVisibility(Visibility::Value ScrollVisibility)
{
    HRESULT hr = S_OK;
    CVisibilityValue* pValue = NULL;

    IFC(CVisibilityValue::Create(ScrollVisibility, &pValue));

    IFC(SetValueReadOnly(&ComputedVerticalScrollBarVisibilityProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

HRESULT CScrollViewer::SetComputedHorizontalScrollBarVisibility(Visibility::Value ScrollVisibility)
{
    HRESULT hr = S_OK;
    CVisibilityValue* pValue = NULL;

    IFC(CVisibilityValue::Create(ScrollVisibility, &pValue));

    IFC(SetValueReadOnly(&ComputedHorizontalScrollBarVisibilityProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

HRESULT CScrollViewer::PostTemplateApplied()
{
    HRESULT hr = S_OK;

    IFC(CContentControl::PostTemplateApplied());

    IFC(GetTemplateChild(L"PART_ScrollContentPresenter", &m_ScrollPresenter));

    IFC(m_ScrollPresenter->SetScrollOwner(this));

Cleanup:
    return hr;
}

HRESULT CScrollViewer::PreTemplateRevoked()
{
    HRESULT hr = S_OK;
    
    if(m_ScrollPresenter != NULL)
    {
        IFC(m_ScrollPresenter->SetScrollOwner(NULL));

        ReleaseObject(m_ScrollPresenter);
    }

    IFC(CContentControl::PreTemplateRevoked());

Cleanup:
    return hr;
}

HRESULT CScrollViewer::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}

void CScrollViewer::OnCommandExecuted(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CRoutedCommandEventArgs* pRoutedCommandEventArgs = NULL;
    CCommand* pCommand = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(CastType(pRoutedEventArgs, &pRoutedCommandEventArgs));

    pCommand = pRoutedCommandEventArgs->GetCommand();
    IFCPTR(pCommand);

    if(pCommand == &CScrollBar::LineUpCommand)
    {
        IFC(LineUp());

        pRoutedCommandEventArgs->SetHandled();
    }
    else if(pCommand == &CScrollBar::LineDownCommand)
    {
        IFC(LineDown());

        pRoutedCommandEventArgs->SetHandled();
    }

Cleanup:
    ;
}

HRESULT CScrollViewer::LineUp()
{
    HRESULT hr = S_OK;

    if(m_ScrollPresenter != NULL)
    {
        IFC(m_ScrollPresenter->LineUp());
    }

Cleanup:
    return hr;
}

HRESULT CScrollViewer::LineDown()
{
    HRESULT hr = S_OK;

    if(m_ScrollPresenter != NULL)
    {
        IFC(m_ScrollPresenter->LineDown());
    }

Cleanup:
    return hr;
}

HRESULT CScrollViewer::InvalidateScrollInformation()
{
    HRESULT hr = S_OK;
    CFloatValue* pValue = NULL;

    if(m_ScrollPresenter != NULL && !m_InMeasure)
    {
        SizeF Extent = m_ScrollPresenter->GetExtent();
        SizeF Viewport = m_ScrollPresenter->GetViewport();

        FLOAT HorizontalOffset = m_ScrollPresenter->GetHorizontalOffset();
        FLOAT VerticalOffset = m_ScrollPresenter->GetVerticalOffset();

        FLOAT ScrollableWidth = std::max(Extent.width - Viewport.width, 0.0f);
        FLOAT ScrollableHeight = std::max(Extent.height - Viewport.height, 0.0f);

        FLOAT currentExtentWidth;
        IFC(GetBasicTypeEffectiveValue(&ExtentWidthProperty, &currentExtentWidth));

        if (!IsFloatNear(Extent.width, currentExtentWidth))
        {
            IFC(CFloatValue::Create(Extent.width, &pValue));
            IFC(SetValueReadOnly(&ExtentWidthProperty, pValue));
            ReleaseObject(pValue);
        }

        FLOAT currentExtentHeight;
        IFC(GetBasicTypeEffectiveValue(&ExtentHeightProperty, &currentExtentHeight));

        if (!IsFloatNear(Extent.height, currentExtentHeight))
        {
            IFC(CFloatValue::Create(Extent.height, &pValue));
            IFC(SetValueReadOnly(&ExtentHeightProperty, pValue));
            ReleaseObject(pValue);
        }

        FLOAT currentViewportWidth;
        IFC(GetBasicTypeEffectiveValue(&ViewportWidthProperty, &currentViewportWidth));

        if (!IsFloatNear(Viewport.width, currentViewportWidth))
        {
            IFC(CFloatValue::Create(Viewport.width, &pValue));
            IFC(SetValueReadOnly(&ViewportWidthProperty, pValue));
            ReleaseObject(pValue);
        }

        FLOAT currentViewportHeight;
        IFC(GetBasicTypeEffectiveValue(&ViewportHeightProperty, &currentViewportHeight));

        if (!IsFloatNear(Viewport.height, currentViewportHeight))
        {
            IFC(CFloatValue::Create(Viewport.height, &pValue));
            IFC(SetValueReadOnly(&ViewportHeightProperty, pValue));
            ReleaseObject(pValue);
        }

        FLOAT currentHorizontalOffset;
        IFC(GetBasicTypeEffectiveValue(&HorizontalOffsetProperty, &currentHorizontalOffset));

        if (!IsFloatNear(HorizontalOffset, currentHorizontalOffset))
        {
            IFC(CFloatValue::Create(HorizontalOffset, &pValue));
            IFC(SetValueReadOnly(&HorizontalOffsetProperty, pValue));
            ReleaseObject(pValue);
        }

        FLOAT currentVerticalOffset;
        IFC(GetBasicTypeEffectiveValue(&VerticalOffsetProperty, &currentVerticalOffset));

        if (!IsFloatNear(VerticalOffset, currentVerticalOffset))
        {
            IFC(CFloatValue::Create(VerticalOffset, &pValue));
            IFC(SetValueReadOnly(&VerticalOffsetProperty, pValue));
            ReleaseObject(pValue);
        }

        FLOAT currentScrollableWidth;
        IFC(GetBasicTypeEffectiveValue(&ScrollableWidthProperty, &currentScrollableWidth));

        if (!IsFloatNear(ScrollableWidth, currentScrollableWidth))
        {
            IFC(CFloatValue::Create(ScrollableWidth, &pValue));
            IFC(SetValueReadOnly(&ScrollableWidthProperty, pValue));
            ReleaseObject(pValue);
        }

        FLOAT currentScrollableHeight;
        IFC(GetBasicTypeEffectiveValue(&ScrollableHeightProperty, &currentScrollableHeight));

        if (!IsFloatNear(ScrollableHeight, currentScrollableHeight))
        {
            IFC(CFloatValue::Create(ScrollableHeight, &pValue));
            IFC(SetValueReadOnly(&ScrollableHeightProperty, pValue));
            ReleaseObject(pValue);
        }
    }

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

HRESULT CScrollViewer::OnExtentHeightChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CScrollViewer::OnExtentWidthChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CScrollViewer::OnViewportWidthChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CScrollViewer::OnViewportHeightChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CScrollViewer::OnHorizontalOffsetChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue )
{
    HRESULT hr = S_OK;
    CFloatValue* pFloatValue = NULL;

    IFCPTR(pNewValue);

    if(m_ScrollPresenter != NULL)
    {
        bool SetValue = true;

        IFC(CastType(pNewValue, &pFloatValue));

        if (pOldValue != NULL)
        {
            CFloatValue* pOldFloatValue = NULL;

            IFC(CastType(pOldValue, &pOldFloatValue));

            SetValue = !IsFloatNear(pOldFloatValue->GetValue(), pFloatValue->GetValue());
        }

        if (SetValue)
        {
            IFC(m_ScrollPresenter->SetHorizontalOffset(pFloatValue->GetValue()));
        }
    }

Cleanup:
    return hr;
}

HRESULT CScrollViewer::OnVerticalOffsetChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue )
{
    HRESULT hr = S_OK;
    CFloatValue* pFloatValue = NULL;

    IFCPTR(pNewValue);

    if(m_ScrollPresenter != NULL)
    {
        bool SetValue = true;

        IFC(CastType(pNewValue, &pFloatValue));

        if (pOldValue != NULL)
        {
            CFloatValue* pOldFloatValue = NULL;

            IFC(CastType(pOldValue, &pOldFloatValue));

            SetValue = !IsFloatNear(pOldFloatValue->GetValue(), pFloatValue->GetValue());
        }

        if (SetValue)
        {
            IFC(m_ScrollPresenter->SetVerticalOffset(pFloatValue->GetValue()));
        }
    }

Cleanup:
    return hr;
}

HRESULT CScrollViewer::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ExtentHeightProperty,
        &ExtentWidthProperty,
        &ViewportWidthProperty,
        &ViewportHeightProperty,
        &HorizontalOffsetProperty,
        &VerticalOffsetProperty,
        &ScrollableWidthProperty,
        &ScrollableHeightProperty,
        &ComputedHorizontalScrollBarVisibilityProperty,
        &ComputedVerticalScrollBarVisibilityProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CContentControl::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

__override __checkReturn HRESULT 
CScrollViewer::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::ScrollViewer)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case ScrollViewerProperties::ExtentHeight:
                {
                    *ppLayeredValue = &m_ExtentHeight;
                    break;
                }

            case ScrollViewerProperties::ExtentWidth:
                {
                    *ppLayeredValue = &m_ExtentWidth;
                    break;
                }

            case ScrollViewerProperties::ViewportWidth:
                {
                    *ppLayeredValue = &m_ViewportWidth;
                    break;
                }

            case ScrollViewerProperties::ViewportHeight:
                {
                    *ppLayeredValue = &m_ViewportHeight;
                    break;
                }

            case ScrollViewerProperties::HorizontalOffset:
                {
                    *ppLayeredValue = &m_HorizontalOffset;
                    break;
                }

            case ScrollViewerProperties::VerticalOffset:
                {
                    *ppLayeredValue = &m_VerticalOffset;
                    break;
                }

            case ScrollViewerProperties::ScrollableWidth:
                {
                    *ppLayeredValue = &m_ScrollableWidth;
                    break;
                }

            case ScrollViewerProperties::ScrollableHeight:
                {
                    *ppLayeredValue = &m_ScrollableHeight;
                    break;
                }

            case ScrollViewerProperties::ComputedHorizontalScrollBarVisibility:
                {
                    *ppLayeredValue = &m_ComputedHorizontalScrollBarVisibility;
                    break;
                }

            case ScrollViewerProperties::ComputedVerticalScrollBarVisibility:
                {
                    *ppLayeredValue = &m_ComputedVerticalScrollBarVisibility;
                    break;
                }

            default:
                {
                    IFC(E_UNEXPECTED);
                }
        }
    }
    else
    {
        IFC_NOTRACE(CContentControl::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

//
// CScrollViewer
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CScrollViewer_TypeIndex()
{
    return TypeIndex::ScrollViewer;
}

extern "C" UIFRAMEWORK_API
CContentControl* CScrollViewer_CastTo_CContentControl(CScrollViewer* pScrollViewer)
{
    return pScrollViewer;
}

extern "C" UIFRAMEWORK_API
CScrollViewer* CObjectWithType_CastTo_CScrollViewer(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::ScrollViewer)) ? (CScrollViewer*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_ExtentHeightProperty()
{
    return &CScrollViewer::ExtentHeightProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_ExtentWidthProperty()
{
    return &CScrollViewer::ExtentWidthProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_ViewportWidthProperty()
{
    return &CScrollViewer::ViewportWidthProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_ViewportHeightProperty()
{
    return &CScrollViewer::ViewportHeightProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_HorizontalOffsetProperty()
{
    return &CScrollViewer::HorizontalOffsetProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_VerticalOffsetProperty()
{
    return &CScrollViewer::VerticalOffsetProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_ScrollableWidthProperty()
{
    return &CScrollViewer::ScrollableWidthProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_ScrollableHeightProperty()
{
    return &CScrollViewer::ScrollableHeightProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_ComputedHorizontalScrollBarVisibilityProperty()
{
    return &CScrollViewer::ComputedHorizontalScrollBarVisibilityProperty;
}

extern "C" UIFRAMEWORK_API
CProperty* CScrollViewer_ComputedVerticalScrollBarVisibilityProperty()
{
    return &CScrollViewer::ComputedVerticalScrollBarVisibilityProperty;
}