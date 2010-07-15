#include "ScrollContentPresenter.h"

CScrollContentPresenter::CScrollContentPresenter() : m_HorizontalOffset(0),
                                                     m_VerticalOffset(0)
{
}

CScrollContentPresenter::~CScrollContentPresenter()
{
}

HRESULT CScrollContentPresenter::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CContentPresenter::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::LineUp()
{
    HRESULT hr = S_OK;

    m_VerticalOffset -= 10;

    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::LineDown()
{
    HRESULT hr = S_OK;

    m_VerticalOffset += 10;

    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

FLOAT CScrollContentPresenter::GetVerticalOffset()
{
    return m_VerticalOffset;
}

FLOAT CScrollContentPresenter::GetHorizontalOffset()
{
    return m_HorizontalOffset;
}

BOOL CScrollContentPresenter::CanScrollHorizontally()
{
    //TODO: Query properties.
    return TRUE;
}

BOOL CScrollContentPresenter::CanScrollVertically()
{
    //TODO: Query properties.
    return TRUE;
}

HRESULT CScrollContentPresenter::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF AvailableContentSize;
    SizeF DesiredContentSize;

    AvailableContentSize.width = CanScrollHorizontally() ? FLT_MAX : AvailableSize.width;
    AvailableContentSize.height = CanScrollVertically() ? FLT_MAX : AvailableSize.height;

    IFC(CContentPresenter::MeasureInternal(AvailableContentSize, DesiredContentSize));

    DesiredSize.width = std::min(DesiredContentSize.width, AvailableSize.width);
    DesiredSize.height = std::min(DesiredContentSize.height, AvailableSize.height);

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    CUIElement* pContentChild = NULL;
    
    IFC(GetContentChild(&pContentChild));

    if(pContentChild != NULL)
    {
        SizeF ArrangePosition(GetHorizontalOffset(), -GetVerticalOffset());

        RectF ArrangeRect = MakeRect(ArrangePosition, AvailableSize);

        IFC(pContentChild->Arrange(ArrangeRect));
    }

    UsedSize = AvailableSize;
      
Cleanup:
    ReleaseObject(pContentChild);

    return hr;
}

HRESULT CScrollContentPresenter::OnContentChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(CContentPresenter::OnContentChanged(pOldValue, pNewValue));

    m_HorizontalOffset = 0;
    m_VerticalOffset = 0;

Cleanup:
    return hr;
}

BOOL CScrollContentPresenter::ShouldClipToLayout()
{
    return TRUE;
}