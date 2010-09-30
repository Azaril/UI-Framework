#include "ScrollContentPresenter.h"

CScrollContentPresenter::CScrollContentPresenter() : m_HorizontalOffset(0),
                                                     m_VerticalOffset(0),
                                                     m_ChildTransformDirty(FALSE),
                                                     m_ScrollOwner(NULL)
{
    m_ChildrenTransform = Matrix3X2F::Identity();
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

    SetScrollOffsets(m_HorizontalOffset, m_VerticalOffset - 10);

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::LineDown()
{
    HRESULT hr = S_OK;

    SetScrollOffsets(m_HorizontalOffset, m_VerticalOffset + 10);

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::DirtyScrollInformation()
{
    HRESULT hr = S_OK;

    if(m_ScrollOwner != NULL)
    {
        IFC(m_ScrollOwner->InvalidateScrollInformation());
    }

    DirtyChildTransform();
    
Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::SetScrollOwner(IScrollOwner* pOwner)
{
    HRESULT hr = S_OK;

    m_ScrollOwner = pOwner;
    
    if(m_ScrollOwner)
    {
        IFC(m_ScrollOwner->InvalidateScrollInformation());
    }

Cleanup:
    return hr;
}

void CScrollContentPresenter::DirtyChildTransform()
{
    m_ChildTransformDirty = TRUE;
}

HRESULT CScrollContentPresenter::SetScrollOffsets(FLOAT XOffset, FLOAT YOffset)
{
    HRESULT hr = S_OK;
    SizeF FinalSize = GetFinalSize();
    
    FLOAT MaxX = std::max((m_Extent.width - FinalSize.width), 0.0f);
    FLOAT MaxY = std::max((m_Extent.height - FinalSize.height), 0.0f);

    m_HorizontalOffset = std::max(std::min(XOffset, MaxX), 0.0f);
    m_VerticalOffset = std::max(std::min(YOffset, MaxY), 0.0f);

    DirtyScrollInformation();

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

const Matrix3X2F* CScrollContentPresenter::GetChildRenderTransform()
{
    return &m_ChildrenTransform;
}

HRESULT CScrollContentPresenter::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    if(m_ChildTransformDirty)
    {
        //TODO: This should track dirtiness and only be changed in the render walk.
        m_ChildrenTransform = Matrix3X2F::Translation(-m_HorizontalOffset, -m_VerticalOffset);
    }

    IFC(CContentPresenter::PreRenderInternal(Context));

Cleanup:
    return hr;
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

HRESULT CScrollContentPresenter::Arrange(RectF Bounds)
{
    HRESULT hr = S_OK;

    IFC(CContentPresenter::Arrange(Bounds));

    IFC(DirtyScrollInformation());

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    CUIElement* pContentChild = NULL;
    SizeF AvailableContentSize;
    SizeF DesiredContentSize;
    
    IFC(GetContentChild(&pContentChild));

    if(pContentChild != NULL)
    {
        SizeF ChildDesiredSize = pContentChild->GetDesiredSize();

        AvailableContentSize.width = CanScrollHorizontally() ?  std::max(AvailableSize.width, ChildDesiredSize.width) : AvailableSize.width;
        AvailableContentSize.height = CanScrollVertically() ? std::max(AvailableSize.height, ChildDesiredSize.height) : AvailableSize.height;

        RectF ArrangeRect = MakeRect(AvailableContentSize);

        IFC(pContentChild->Arrange(ArrangeRect));
    }

    m_Extent = AvailableContentSize;

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

SizeF CScrollContentPresenter::GetExtent()
{
    return m_Extent;
}

SizeF CScrollContentPresenter::GetViewport()
{
    return GetFinalSize();
}