#include "ScrollBar.h"
#include "DelegatingPropertyInformation.h"
#include "StaticCommandInformation.h"
#include "DelegatingCommandInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT( Orientation, COrientationValue, Orientation::Vertical );

//
// Properties
//
CStaticProperty CScrollBar::OrientationProperty( L"Orientation", TypeIndex::Orientation, StaticPropertyFlags::None, &GET_DEFAULT( Orientation ), &INSTANCE_CHANGE_CALLBACK( CScrollBar, OnOrientationChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollBar, OnOrientationChanged );

//
// Commands
//
CStaticRoutedCommand CScrollBar::LineUpCommand( L"LineUp" );
CStaticRoutedCommand CScrollBar::LineDownCommand( L"LineDown" );

CScrollBar::CScrollBar() : m_Orientation(this, &CScrollBar::OrientationProperty)
{
}

CScrollBar::~CScrollBar()
{
}

HRESULT CScrollBar::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CControl::Initialize(pProviders));

Cleanup:
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
    else
    {
        hr = CControl::GetLayeredValue(pProperty, ppLayeredValue);
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
        &OrientationProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
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
    IFC(CControl::CreateCommandInformation(&pBaseInformation));
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

HRESULT CScrollBar::GetEffectiveOrientation(Orientation::Value* pOrientation)
{
    HRESULT hr = S_OK;
    COrientationValue* pEffectiveValue = NULL;

    IFCPTR(pOrientation);

    IFC(m_Orientation.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pOrientation = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

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