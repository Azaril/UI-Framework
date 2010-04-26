#include "Border.h"

CBorder::CBorder() : m_BorderVisual(NULL)
{
    m_BorderThickness.left = 0;
    m_BorderThickness.top = 0;
    m_BorderThickness.right = 0;
    m_BorderThickness.bottom = 0;
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
    }

Cleanup:
    return hr;
}

HRESULT CBorder::SetChild( CUIElement* pChild )
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

HRESULT CBorder::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF InternalSize = { 0 };
    SizeF BaseSize = { 0 };
    SizeF InternalSizeDesired = { 0 };

    IFC(CDecorator::MeasureInternal(AvailableSize, BaseSize));

    InternalSize.width = max(AvailableSize.width - (m_BorderThickness.left + m_BorderThickness.right), 0);
    InternalSize.height = max(AvailableSize.height - (m_BorderThickness.top + m_BorderThickness.bottom), 0);

    if(m_Child != NULL)
    {
        IFC(m_Child->Measure(InternalSize));

        InternalSizeDesired = m_Child->GetDesiredSize();
    }

    DesiredSize.width = max(InternalSizeDesired.width + (m_BorderThickness.left + m_BorderThickness.right), BaseSize.width);
    DesiredSize.height = max(InternalSizeDesired.height + (m_BorderThickness.top + m_BorderThickness.bottom), BaseSize.height);

Cleanup:
    return hr;
}

HRESULT CBorder::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    IFC(m_BorderVisual->SetSize(Size));

    if(m_Child != NULL)
    {
        SizeF InternalSize = { Size.width - (m_BorderThickness.left + m_BorderThickness.right), Size.height - (m_BorderThickness.top + m_BorderThickness.bottom) };
        Matrix3X2 Transform = D2D1::Matrix3x2F::Translation(m_BorderThickness.left, m_BorderThickness.top);
        
        InternalSize.width = max(InternalSize.width, 0);
        InternalSize.height = max(InternalSize.height, 0);

        IFC(m_Child->Arrange(InternalSize));

        IFC(m_Child->SetVisualTransform(Transform));
    }
      
Cleanup:
    return hr;
}