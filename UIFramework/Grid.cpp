#include "Grid.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

CGrid::CGrid()
{
}

CGrid::~CGrid()
{
}

HRESULT CGrid::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CPanel::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CGrid::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    //TODO: Implement correctly.
    for(UINT32 i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        IFC(pElement->Measure(AvailableSize));
    }


    DesiredSize = AvailableSize;

Cleanup:
    return hr;
}

HRESULT CGrid::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    Point2F LayoutPoint;
    CUIElementCollection* pChildCollection = NULL;

    pChildCollection = GetChildCollection();
    IFCPTR(pChildCollection);

    //TODO: Implement correctly.
    for(UINT32 i = 0; i < pChildCollection->GetCount(); i++)
    {
        CUIElement* pElement = pChildCollection->GetAtIndex(i);

        IFC(pElement->Arrange(MakeRect(AvailableSize)));
    }

    UsedSize = AvailableSize;

Cleanup:
    return hr;
}

//HRESULT CGrid::CreatePropertyInformation(CPropertyInformation **ppInformation)
//{
//    HRESULT hr = S_OK;
//    CStaticPropertyInformation* pStaticInformation = NULL;
//    CPropertyInformation* pBaseInformation = NULL;
//    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;
//
//    CStaticProperty* Properties[] = 
//    {
//        &OrientationProperty
//    };
//    
//    IFCPTR(ppInformation);
//
//    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
//    IFC(CPanel::CreatePropertyInformation(&pBaseInformation));
//    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));
//
//    *ppInformation = pDelegatingPropertyInformation;
//    pDelegatingPropertyInformation = NULL;
//
//Cleanup:
//    ReleaseObject(pStaticInformation);
//    ReleaseObject(pBaseInformation);
//    ReleaseObject(pDelegatingPropertyInformation);
//
//    return hr;
//}

//HRESULT CGrid::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
//{
//    HRESULT hr = S_OK;
//
//    IFCPTR(pProperty);
//    IFCPTR(ppLayeredValue);
//
//    //TODO: Make this a lookup table rather than requiring a comparison per property.
//    if(pProperty == &CGrid::OrientationProperty)
//    {
//        *ppLayeredValue = &m_Orientation;
//    }
//    else
//    {
//        hr = CPanel::GetLayeredValue(pProperty, ppLayeredValue);
//    }
//
//Cleanup:
//    return hr;
//}

//
// CGrid
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CGrid_TypeIndex()
{
    return TypeIndex::Grid;
}

extern "C" UIFRAMEWORK_API
CPanel* CGrid_CastTo_CPanel(CGrid* pGrid)
{
    return pGrid;
}

extern "C" UIFRAMEWORK_API
CGrid* CObjectWithType_CastTo_CGrid(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::Grid)) ? (CGrid*)pObject : NULL;
}