#include "Canvas.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//StaticClassProperty CanvasProperties[] =
//{
//};
//
//StaticClassProperties CanvasPropertyInformation =
//{
//    CanvasProperties,
//    ARRAYSIZE(CanvasProperties)
//};

CCanvas::CCanvas()
{
}

CCanvas::~CCanvas()
{
}

HRESULT CCanvas::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CPanel::Initialize());

Cleanup:
    return hr;
}

HRESULT CCanvas::AddChild(CUIElement* pElement)
{
    HRESULT hr = S_OK;
    CCanvasLayoutInformation* pLayoutInformation = NULL;

    IFCPTR(pElement);

    IFC(CPanel::AddChild(pElement));

    IFC(CCanvasLayoutInformation::Create(pElement, &pLayoutInformation));

    m_LayoutInformation.push_back(pLayoutInformation);

Cleanup:
    return hr;
}

HRESULT CCanvas::RemoveChild(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    IFC(CPanel::RemoveChild(pElement));

    for(LayoutInformationCollection::iterator It = m_LayoutInformation.begin(); It != m_LayoutInformation.end(); ++It)
    {
        if((*It)->GetUIElement() == pElement)
        {
            (*It)->Release();

            m_LayoutInformation.erase(It);

            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CCanvas::SetChildPosition(CUIElement* pElement, Point2F Position)
{
    HRESULT hr = S_OK;
    CCanvasLayoutInformation* pLayoutInformation = NULL;

    IFCPTR(pElement);

    IFC(GetChildLayout(pElement, &pLayoutInformation));

    IFC(pLayoutInformation->SetPosition(Position));

Cleanup:
    ReleaseObject(pLayoutInformation);

    return hr;
}

HRESULT CCanvas::GetChildLayout(CUIElement* pElement, CCanvasLayoutInformation** ppLayoutInfo)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);
    IFCPTR(ppLayoutInfo);

    for(LayoutInformationCollection::iterator It = m_LayoutInformation.begin(); It != m_LayoutInformation.end(); ++It)
    {
        if((*It)->GetUIElement() == pElement)
        {
            *ppLayoutInfo = *It;

            AddRefObject(*It);

            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CCanvas::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF BaseDesiredSize = { 0 };
    SizeF MaxSize = { FLT_MAX, FLT_MAX };
    SizeF MaxSizeNeeded = { 0, 0 };

    IFC(CPanel::MeasureInternal(AvailableSize, BaseDesiredSize));

    MaxSizeNeeded = BaseDesiredSize;

    for(LayoutInformationCollection::iterator It = m_LayoutInformation.begin(); It != m_LayoutInformation.end(); ++It)
    {
        CUIElement* pElement = (*It)->GetUIElement();

        IFC(pElement->Measure(MaxSize));

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        Point2F ElementPosition = (*It)->GetPosition();
        SizeF ElementExtentNeeded = { ElementPosition.x + ElementDesiredSize.width, ElementPosition.y + ElementDesiredSize.height };

        MaxSizeNeeded.width = max(MaxSizeNeeded.width, ElementExtentNeeded.width);
        MaxSizeNeeded.height = max(MaxSizeNeeded.height, ElementExtentNeeded.height);
    }

    DesiredSize = MaxSizeNeeded;

Cleanup:
    return hr;
}

HRESULT CCanvas::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;

    for(LayoutInformationCollection::iterator It = m_LayoutInformation.begin(); It != m_LayoutInformation.end(); ++It)
    {
        CUIElement* pElement = (*It)->GetUIElement();

        SizeF ElementDesiredSize = pElement->GetDesiredSize();
        Point2F ElementPosition = (*It)->GetPosition();

        Matrix3X2 VisualTransform = D2D1::Matrix3x2F::Translation(ElementPosition.x, ElementPosition.y);
        
        IFC(pElement->SetVisualTransform(VisualTransform));

        IFC(pElement->Arrange(ElementDesiredSize));
    }

    IFC(CPanel::ArrangeInternal(Size));

Cleanup:
    return hr;
}

//HRESULT CCanvas::CreatePropertyInformation(CPropertyInformation** ppInformation)
//{
//    HRESULT hr = S_OK;
//    CStaticPropertyInformation* pStaticInformation = NULL;
//    CPropertyInformation* pBaseInformation = NULL;
//    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;
//
//    IFCPTR(ppInformation);
//
//    IFC(CStaticPropertyInformation::Create(&CanvasPropertyInformation, &pStaticInformation));
//    IFC(CPanel::CreatePropertyInformation(&pBaseInformation));
//    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));
//
//    *ppInformation = pDelegatingProperyInformation;
//    pDelegatingProperyInformation = NULL;
//
//Cleanup:
//    ReleaseObject(pStaticInformation);
//    ReleaseObject(pBaseInformation);
//    ReleaseObject(pDelegatingProperyInformation);
//
//    return hr;
//}

CCanvasLayoutInformation::CCanvasLayoutInformation() : m_Element(NULL)
{
    m_Position.x = 0;
    m_Position.y = 0;
}

CCanvasLayoutInformation::~CCanvasLayoutInformation()
{
    ReleaseObject(m_Element);
}

HRESULT CCanvasLayoutInformation::Initialize(CUIElement* pElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pElement);

    m_Element = pElement;
    AddRefObject(m_Element);

Cleanup:
    return hr;
}

CUIElement* CCanvasLayoutInformation::GetUIElement()
{
    return m_Element;
}

Point2F CCanvasLayoutInformation::GetPosition()
{
    return m_Position;
}

HRESULT CCanvasLayoutInformation::SetPosition(Point2F Position)
{
    HRESULT hr = S_OK;

    m_Position = Position;

    return hr;
}