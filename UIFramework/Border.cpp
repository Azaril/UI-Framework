#include "Border.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

StaticClassProperty BorderProperties[] =
{
    { L"Padding", FALSE, TypeIndex::RectF }
};

StaticClassProperties BorderPropertyInformation =
{
    BorderProperties,
    ARRAYSIZE(BorderProperties)
};

CBorder::CBorder() : m_BorderVisual(NULL)
{
    m_BorderThickness.left = 0;
    m_BorderThickness.top = 0;
    m_BorderThickness.right = 0;
    m_BorderThickness.bottom = 0;

    m_Padding.left = 0;
    m_Padding.top = 0;
    m_Padding.right = 0;
    m_Padding.bottom = 0;
}

CBorder::~CBorder()
{
    Finalize();
}

HRESULT CBorder::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CDecorator::Initialize());

    IFC(CRectangleVisual::Create(&m_BorderVisual));

    IFC(AddChildVisual(m_BorderVisual));

Cleanup:
    return hr;
}

HRESULT CBorder::Finalize()
{
    HRESULT hr = S_OK;

    if(m_BorderVisual)
    {
        IFC(RemoveChildVisual(m_BorderVisual));

        ReleaseObject(m_BorderVisual);
    }

Cleanup:
    return hr;
}

HRESULT CBorder::SetChild(CUIElement* pChild)
{
    HRESULT hr = S_OK;

    IFC(CDecorator::SetChild(pChild));

    IFC(MoveToBack(m_BorderVisual));

Cleanup:
    return hr;
}

HRESULT CBorder::SetBackgroundBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFC(m_BorderVisual->SetBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::SetBorderThickness(const RectF& Border)
{
    HRESULT hr = S_OK;

    m_BorderThickness = Border;

    IFC(m_BorderVisual->SetBorderThickness(Border));

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CBorder::SetBorderBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFC(m_BorderVisual->SetOutlineBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::SetPadding(const RectF& Padding)
{
    HRESULT hr = S_OK;

    m_Padding = Padding;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CBorder::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF InternalSize = { 0 };
    SizeF BaseSize = { 0 };
    SizeF InternalSizeDesired = { 0 };

    IFC(CDecorator::MeasureInternal(AvailableSize, BaseSize));

    InternalSize.width = max(AvailableSize.width - (m_BorderThickness.left + m_BorderThickness.right) - (m_Padding.left + m_Padding.right), 0);
    InternalSize.height = max(AvailableSize.height - (m_BorderThickness.top + m_BorderThickness.bottom) - (m_Padding.top + m_Padding.bottom), 0);

    if(m_Child != NULL)
    {
        IFC(m_Child->Measure(InternalSize));

        InternalSizeDesired = m_Child->GetDesiredSize();
    }

    DesiredSize.width = max(InternalSizeDesired.width + (m_BorderThickness.left + m_BorderThickness.right) + (m_Padding.left + m_Padding.right), BaseSize.width);
    DesiredSize.height = max(InternalSizeDesired.height + (m_BorderThickness.top + m_BorderThickness.bottom) + (m_Padding.top + m_Padding.bottom), BaseSize.height);

Cleanup:
    return hr;
}

HRESULT CBorder::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    IFC(m_BorderVisual->SetSize(Size));

    if(m_Child != NULL)
    {
        SizeF InternalSize = { Size.width - (m_BorderThickness.left + m_BorderThickness.right) - (m_Padding.left + m_Padding.right), 
                               Size.height - (m_BorderThickness.top + m_BorderThickness.bottom)- (m_Padding.top + m_Padding.bottom) };

        Matrix3X2 Transform = D2D1::Matrix3x2F::Translation(m_BorderThickness.left + m_Padding.left, m_BorderThickness.top + m_Padding.top);
        
        InternalSize.width = max(InternalSize.width, 0);
        InternalSize.height = max(InternalSize.height, 0);

        IFC(m_Child->Arrange(InternalSize));

        IFC(m_Child->SetVisualTransform(Transform));
    }

    IFC(CDecorator::Arrange(Size));
      
Cleanup:
    return hr;
}

HRESULT CBorder::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(&BorderPropertyInformation, &pStaticInformation));
    IFC(CDecorator::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}