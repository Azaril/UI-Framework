#include "ScrollViewer.h"
#include "RoutedCommand.h"
#include "RoutedCommandEventArgs.h"
#include "ScrollBar.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT( ExtentHeight, CFloatValue, 0.0f );
DEFINE_GET_DEFAULT( ExtentWidth, CFloatValue, 0.0f );
DEFINE_GET_DEFAULT( ViewportWidth, CFloatValue, 0.0f );
DEFINE_GET_DEFAULT( ViewportHeight, CFloatValue, 0.0f );
DEFINE_GET_DEFAULT( HorizontalOffset, CFloatValue, 0.0f );
DEFINE_GET_DEFAULT( VerticalOffset, CFloatValue, 0.0f );
DEFINE_GET_DEFAULT( ScrollableWidth, CFloatValue, 0.0f );
DEFINE_GET_DEFAULT( ScrollableHeight, CFloatValue, 0.0f );
DEFINE_GET_DEFAULT( ComputedVerticalScrollBarVisibility, CVisibilityValue, Visibility::Collapsed );
DEFINE_GET_DEFAULT( ComputedHorizontalScrollBarVisibility, CVisibilityValue, Visibility::Collapsed );

//
// Properties
//
CStaticProperty CScrollViewer::ExtentHeightProperty( L"ExtentHeight", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ExtentHeight ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnExtentHeightChanged ) );
CStaticProperty CScrollViewer::ExtentWidthProperty( L"ExtentWidth", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ExtentWidth ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnExtentWidthChanged ) );
CStaticProperty CScrollViewer::ViewportWidthProperty( L"ViewportWidth", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ViewportWidth ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnViewportWidthChanged ) );
CStaticProperty CScrollViewer::ViewportHeightProperty( L"ViewportHeight", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ViewportHeight ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnViewportHeightChanged ) );
CStaticProperty CScrollViewer::HorizontalOffsetProperty( L"HorizontalOffset", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( HorizontalOffset ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnHorizontalOffsetChanged ) );
CStaticProperty CScrollViewer::VerticalOffsetProperty( L"VerticalOffset", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT( VerticalOffset ), &INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnVerticalOffsetChanged ) );
CStaticProperty CScrollViewer::ScrollableWidthProperty( L"ScrollableWidth", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ScrollableWidth ) );
CStaticProperty CScrollViewer::ScrollableHeightProperty( L"ScrollableHeight", TypeIndex::Float, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ScrollableHeight ) );
CStaticProperty CScrollViewer::ComputedHorizontalScrollBarVisibilityProperty( L"ComputedHorizontalScrollBarVisibility", TypeIndex::Visibility, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ComputedVerticalScrollBarVisibility ) );
CStaticProperty CScrollViewer::ComputedVerticalScrollBarVisibilityProperty( L"ComputedVerticalScrollBarVisibility", TypeIndex::Visibility, StaticPropertyFlags::ReadOnly, &GET_DEFAULT( ComputedHorizontalScrollBarVisibility ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnExtentHeightChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnExtentWidthChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnViewportWidthChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnViewportHeightChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnHorizontalOffsetChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollViewer, OnVerticalOffsetChanged );

CScrollViewer::CScrollViewer() : m_ExtentHeight(this, &CScrollViewer::ExtentHeightProperty),
                                 m_ExtentWidth(this, &CScrollViewer::ExtentWidthProperty),
                                 m_ViewportWidth(this, &CScrollViewer::ViewportWidthProperty),
                                 m_ViewportHeight(this, &CScrollViewer::ViewportHeightProperty),
                                 m_HorizontalOffset(this, &CScrollViewer::HorizontalOffsetProperty),
                                 m_VerticalOffset(this, &CScrollViewer::VerticalOffsetProperty),
                                 m_ScrollableWidth(this, &CScrollViewer::ScrollableWidthProperty),
                                 m_ScrollableHeight(this, &CScrollViewer::ScrollableHeightProperty),
                                 m_ComputedHorizontalScrollBarVisibility(this, &CScrollViewer::ComputedHorizontalScrollBarVisibilityProperty),
                                 m_ComputedVerticalScrollBarVisibility(this, &CScrollViewer::ComputedVerticalScrollBarVisibilityProperty),
                                 m_ScrollPresenter(NULL),
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

HRESULT CScrollViewer::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    CUIElement* pChild = NULL;
    ScrollBarVisibility::Value ScrollVerticalVisibility;
    ScrollBarVisibility::Value ScrollHorizontalVisibility;
    SizeF ElementDesiredSize;

    IFC(GetEffectiveVerticalScrollBarVisibility(&ScrollVerticalVisibility));
    IFC(GetEffectiveHorizontalScrollBarVisibility(&ScrollHorizontalVisibility));

    {
        BOOL AutoVerticalVisibility = (ScrollVerticalVisibility == ScrollBarVisibility::Auto);
        BOOL AutoHorizontalVisibility = (ScrollHorizontalVisibility == ScrollBarVisibility::Auto);
        BOOL VerticalDisableScrollBar = (ScrollVerticalVisibility == ScrollBarVisibility::Disabled);
        BOOL HorizontalDisableScrollBar = (ScrollHorizontalVisibility == ScrollBarVisibility::Disabled);
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

            BOOL MakeHorizontalBarVisible = AutoHorizontalVisibility && (CurrentExtent.width > CurrentViewport.width);
            BOOL MakeVerticalBarVisible = AutoVerticalVisibility && (CurrentExtent.height > CurrentViewport.height);

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

                BOOL SecondPassMakeHorizontalBarVisible = !MakeHorizontalBarVisible && (SecondPassCurrentExtent.width > SecondPassCurrentViewport.width);
                BOOL SecondPassMakeVerticalBarVisible = !MakeVerticalBarVisible && (SecondPassCurrentExtent.height > SecondPassCurrentViewport.height);

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

HRESULT CScrollViewer::GetEffectiveVerticalScrollBarVisibility(ScrollBarVisibility::Value* pVisibility)
{
    HRESULT hr = S_OK;

    //TODO: Use property.
    *pVisibility = ScrollBarVisibility::Auto;

    return hr;
}

HRESULT CScrollViewer::GetEffectiveHorizontalScrollBarVisibility(ScrollBarVisibility::Value* pVisibility)
{
    HRESULT hr = S_OK;

    //TODO: Use property.
    *pVisibility = ScrollBarVisibility::Auto;

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

        IFC(CFloatValue::Create(Extent.width, &pValue));
        IFC(SetValueReadOnly(&ExtentWidthProperty, pValue));
        ReleaseObject(pValue);

        IFC(CFloatValue::Create(Extent.height, &pValue));
        IFC(SetValueReadOnly(&ExtentHeightProperty, pValue));
        ReleaseObject(pValue);

        IFC(CFloatValue::Create(Viewport.width, &pValue));
        IFC(SetValueReadOnly(&ViewportWidthProperty, pValue));
        ReleaseObject(pValue);

        IFC(CFloatValue::Create(Viewport.height, &pValue));
        IFC(SetValueReadOnly(&ViewportHeightProperty, pValue));
        ReleaseObject(pValue);

        IFC(CFloatValue::Create(HorizontalOffset, &pValue));
        IFC(SetValueReadOnly(&HorizontalOffsetProperty, pValue));
        ReleaseObject(pValue);

        IFC(CFloatValue::Create(VerticalOffset, &pValue));
        IFC(SetValueReadOnly(&VerticalOffsetProperty, pValue));
        ReleaseObject(pValue);

        IFC(CFloatValue::Create(ScrollableWidth, &pValue));
        IFC(SetValueReadOnly(&ScrollableWidthProperty, pValue));
        ReleaseObject(pValue);

        IFC(CFloatValue::Create(ScrollableHeight, &pValue));
        IFC(SetValueReadOnly(&ScrollableHeightProperty, pValue));
        ReleaseObject(pValue);
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
        IFC(CastType(pNewValue, &pFloatValue));

        IFC(m_ScrollPresenter->SetHorizontalOffset(pFloatValue->GetValue()));
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
        IFC(CastType(pNewValue, &pFloatValue));

        IFC(m_ScrollPresenter->SetVerticalOffset(pFloatValue->GetValue()));
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

HRESULT CScrollViewer::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CScrollViewer::ExtentHeightProperty)
    {
        *ppLayeredValue = &m_ExtentHeight;
    }
    else if(pProperty == &CScrollViewer::ExtentWidthProperty)
    {
        *ppLayeredValue = &m_ExtentWidth;
    }
    else if(pProperty == &CScrollViewer::ViewportWidthProperty)
    {
        *ppLayeredValue = &m_ViewportWidth;
    }
    else if(pProperty == &CScrollViewer::ViewportHeightProperty)
    {
        *ppLayeredValue = &m_ViewportHeight;
    }
    else if(pProperty == &CScrollViewer::HorizontalOffsetProperty)
    {
        *ppLayeredValue = &m_HorizontalOffset;
    }
    else if(pProperty == &CScrollViewer::VerticalOffsetProperty)
    {
        *ppLayeredValue = &m_VerticalOffset;
    }
    else if(pProperty == &CScrollViewer::ScrollableWidthProperty)
    {
        *ppLayeredValue = &m_ScrollableWidth;
    }
    else if(pProperty == &CScrollViewer::ScrollableHeightProperty)
    {
        *ppLayeredValue = &m_ScrollableHeight;
    }
    else if(pProperty == &CScrollViewer::ComputedHorizontalScrollBarVisibilityProperty)
    {
        *ppLayeredValue = &m_ComputedHorizontalScrollBarVisibility;
    }
    else if(pProperty == &CScrollViewer::ComputedVerticalScrollBarVisibilityProperty)
    {
        *ppLayeredValue = &m_ComputedVerticalScrollBarVisibility;
    }
    else
    {
        hr = CContentControl::GetLayeredValue(pProperty, ppLayeredValue);
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