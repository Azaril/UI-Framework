#include "ScrollContentPresenter.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT( CanScrollVertically, CBoolValue, FALSE );
DEFINE_GET_DEFAULT( CanScrollHorizontally, CBoolValue, FALSE );

//
// Properties
//
CStaticProperty CScrollContentPresenter::CanScrollVerticallyProperty( L"CanScrollVertically", TypeIndex::Bool, StaticPropertyFlags::None, &GET_DEFAULT( CanScrollVertically ), &INSTANCE_CHANGE_CALLBACK( CScrollContentPresenter, OnCanScrollVerticallyChanged ) );
CStaticProperty CScrollContentPresenter::CanScrollHorizontallyProperty( L"CanScrollHorizontally", TypeIndex::Bool, StaticPropertyFlags::None, &GET_DEFAULT( CanScrollHorizontally ), &INSTANCE_CHANGE_CALLBACK( CScrollContentPresenter, OnCanScrollHorizontallyChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollContentPresenter, OnCanScrollVerticallyChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CScrollContentPresenter, OnCanScrollHorizontallyChanged );

CScrollContentPresenter::CScrollContentPresenter() : m_CanScrollVertically(this, &CScrollContentPresenter::CanScrollVerticallyProperty),
                                                     m_CanScrollHorizontally(this, &CScrollContentPresenter::CanScrollHorizontallyProperty),
                                                     m_HorizontalOffset(0),
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

    return hr;
}

HRESULT CScrollContentPresenter::LineDown()
{
    HRESULT hr = S_OK;

    SetScrollOffsets(m_HorizontalOffset, m_VerticalOffset + 10);

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

HRESULT CScrollContentPresenter::SetHorizontalOffset(FLOAT Offset)
{
    HRESULT hr = S_OK;
    
    IFC(SetScrollOffsets(Offset, m_VerticalOffset));

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::SetVerticalOffset(FLOAT Offset)
{
    HRESULT hr = S_OK;
    
    IFC(SetScrollOffsets(m_HorizontalOffset, Offset));

Cleanup:
    return hr;
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

const Matrix3X2F* CScrollContentPresenter::GetChildRenderTransform()
{
    return &m_ChildrenTransform;
}

HRESULT CScrollContentPresenter::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    if(m_ChildTransformDirty)
    {
        m_ChildTransformDirty = FALSE;

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
    bool CanScrollHorizontally = FALSE;
    bool CanScrollVertically = FALSE;

    IFC(GetEffectiveCanScrollHorizontally(&CanScrollHorizontally));
    IFC(GetEffectiveCanScrollVertically(&CanScrollVertically));

    AvailableContentSize.width = CanScrollHorizontally ? FLT_MAX : AvailableSize.width;
    AvailableContentSize.height = CanScrollVertically ? FLT_MAX : AvailableSize.height;

    IFC(CContentPresenter::MeasureInternal(AvailableContentSize, DesiredContentSize));

    DesiredSize.width = std::min(DesiredContentSize.width, AvailableSize.width);
    DesiredSize.height = std::min(DesiredContentSize.height, AvailableSize.height);

    m_Extent = DesiredContentSize;
    m_Viewport = AvailableSize;

    IFC(DirtyScrollInformation());

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
    bool CanScrollHorizontally = FALSE;
    bool CanScrollVertically = FALSE;

    IFC(GetEffectiveCanScrollHorizontally(&CanScrollHorizontally));
    IFC(GetEffectiveCanScrollVertically(&CanScrollVertically));
    
    IFC(GetContentChild(&pContentChild));

    if(pContentChild != NULL)
    {
        SizeF ChildDesiredSize = pContentChild->GetDesiredSize();

        AvailableContentSize.width = CanScrollHorizontally ?  std::max(AvailableSize.width, ChildDesiredSize.width) : AvailableSize.width;
        AvailableContentSize.height = CanScrollVertically ? std::max(AvailableSize.height, ChildDesiredSize.height) : AvailableSize.height;

        RectF ArrangeRect = MakeRect(AvailableContentSize);

        IFC(pContentChild->Arrange(ArrangeRect));
    }

    m_Extent = AvailableContentSize;
    m_Viewport = AvailableSize;

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

bool CScrollContentPresenter::ShouldClipToLayout()
{
    return TRUE;
}

SizeF CScrollContentPresenter::GetExtent()
{
    return m_Extent;
}

SizeF CScrollContentPresenter::GetViewport()
{
    return m_Viewport;
}

HRESULT CScrollContentPresenter::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &CanScrollVerticallyProperty,
        &CanScrollHorizontallyProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CContentPresenter::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CScrollContentPresenter::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CScrollContentPresenter::CanScrollVerticallyProperty)
    {
        *ppLayeredValue = &m_CanScrollVertically;
    }
    else if(pProperty == &CScrollContentPresenter::CanScrollHorizontallyProperty)
    {
        *ppLayeredValue = &m_CanScrollHorizontally;
    }
    else
    {
        hr = CContentPresenter::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::SetCanScrollVertically(bool CanScroll)
{
    HRESULT hr = S_OK;
    CBoolValue* pValue = NULL;

    IFC(CBoolValue::Create(CanScroll, &pValue));

    IFC(SetValue(&CanScrollVerticallyProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

HRESULT CScrollContentPresenter::SetCanScrollHorizontally( bool CanScroll )
{
    HRESULT hr = S_OK;
    CBoolValue* pValue = NULL;

    IFC(CBoolValue::Create(CanScroll, &pValue));

    IFC(SetValue(&CanScrollVerticallyProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

HRESULT CScrollContentPresenter::GetEffectiveCanScrollVertically(bool* pCanScroll)
{
    HRESULT hr = S_OK;
    CBoolValue* pEffectiveValue = NULL;

    IFC(m_CanScrollVertically.GetTypedEffectiveValue(&pEffectiveValue));

    *pCanScroll = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CScrollContentPresenter::GetEffectiveCanScrollHorizontally(bool* pCanScroll)
{
    HRESULT hr = S_OK;
    CBoolValue* pEffectiveValue = NULL;

    IFC(m_CanScrollHorizontally.GetTypedEffectiveValue(&pEffectiveValue));

    *pCanScroll = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CScrollContentPresenter::OnCanScrollVerticallyChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CScrollContentPresenter::OnCanScrollHorizontallyChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}