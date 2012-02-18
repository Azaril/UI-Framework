#include "ScrollBar.h"
#include "DelegatingPropertyInformation.h"
#include "StaticCommandInformation.h"
#include "DelegatingCommandInformation.h"
#include "Binding.h"

//
// Properties
//
namespace ScrollBarProperties
{
    enum Value
    {
        Orientation,
        ViewportSize
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Orientation, Orientation::Value, Orientation::Vertical );
DEFINE_GET_DEFAULT( ViewportSize, FLOAT, 0 );

//
// Properties
//
CStaticProperty CScrollBar::OrientationProperty(TypeIndex::ScrollBar, ScrollBarProperties::Orientation, L"Orientation", TypeIndex::Orientation, StaticPropertyFlags::None, &GET_DEFAULT( Orientation ), &INSTANCE_CHANGE_CALLBACK( CScrollBar, OnOrientationChanged ));
CStaticProperty CScrollBar::ViewportSizeProperty(TypeIndex::ScrollBar, ScrollBarProperties::ViewportSize, L"ViewportSize", TypeIndex::Orientation, StaticPropertyFlags::None, &GET_DEFAULT( ViewportSize ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollBar, OnOrientationChanged );

//
// Commands
//
CStaticRoutedCommand CScrollBar::LineUpCommand( L"LineUp" );
CStaticRoutedCommand CScrollBar::LineDownCommand( L"LineDown" );

CScrollBar::CScrollBar() : m_Track(NULL)
{
}

CScrollBar::~CScrollBar()
{
    ReleaseObject(m_Track);
}

HRESULT CScrollBar::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CRangeBase::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CScrollBar::PostTemplateApplied()
{
    HRESULT hr = S_OK;
    //CBinding* pBinding = NULL;

    IFC(CControl::PostTemplateApplied());

    IFC(GetTemplateChild(L"PART_Track", &m_Track));

    IFCPTR(m_Track);

    //TODO: Does this need a binding?
    //IFC(CBinding::Create(GetProviders(), &pBinding));

    //IFC(m_Track->SetBinding(&CTrack::ValueProperty, pBinding));

Cleanup:
    //ReleaseObject(pBinding);

    return hr;
}

HRESULT CScrollBar::PreTemplateRevoked()
{
    HRESULT hr = S_OK;

    if(m_Track)
    {
        //TODO: Clear binding.

        ReleaseObject(m_Track);
    }    

    return hr;
}

HRESULT CScrollBar::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CScrollBar::OrientationProperty)
    {
        *ppLayeredValue = &m_Orientation;
    }
    else if(pProperty == &CScrollBar::ViewportSizeProperty)
    {
        *ppLayeredValue = &m_ViewportSize;
    }
    else
    {
        hr = CRangeBase::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CScrollBar::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &OrientationProperty,
        &ViewportSizeProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CRangeBase::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CScrollBar::CreateCommandInformation(CCommandInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticCommandInformation* pStaticInformation = NULL;
    CCommandInformation* pBaseInformation = NULL;
    CDelegatingCommandInformation* pDelegatingCommandInformation = NULL;

    CCommand* Commands[] = 
    {
        &LineUpCommand,
        &LineDownCommand
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticCommandInformation::Create(Commands, ARRAYSIZE(Commands), &pStaticInformation))
    IFC(CRangeBase::CreateCommandInformation(&pBaseInformation));
    IFC(CDelegatingCommandInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingCommandInformation));

    *ppInformation = pDelegatingCommandInformation;
    pDelegatingCommandInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingCommandInformation);

    return hr;
}

HRESULT CScrollBar::OnOrientationChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CScrollBar::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
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