#include "UIElement.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

StaticClassProperty UIElementProperties[] =
{
    { L"Width", TypeIndex::Float, StaticPropertyFlags::None },
    { L"Height", TypeIndex::Float, StaticPropertyFlags::None }
};

StaticClassProperties UIElementPropertyInformation =
{
    UIElementProperties,
    ARRAYSIZE(UIElementProperties)
};

CUIElement::CUIElement() : m_Attached(FALSE),
                           m_MeasureDirty(TRUE),
                           m_ArrangeDirty(TRUE),
                           m_PropertyInformation(NULL)
{
    m_Size.width = 0;
    m_Size.height = 0;

    m_DesiredSize.width = 0;
    m_DesiredSize.height = 0;

    m_LastMeasureSize.width = 0;
    m_LastMeasureSize.height = 0;

    m_FinalSize.width = 0;
    m_FinalSize.width = 0;
}

CUIElement::~CUIElement()
{
    ReleaseObject(m_PropertyInformation);
}

HRESULT CUIElement::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CVisual::Initialize());
    
Cleanup:
    return hr;
}

HRESULT CUIElement::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFCEXPECT(!IsAttached());

    m_Attached = TRUE;
    m_Context = Context;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());
    
Cleanup:
    return hr;
}

HRESULT CUIElement::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFCEXPECT(IsAttached());

    m_Attached = FALSE;
    
Cleanup:
    return hr;
}

BOOL CUIElement::IsAttached()
{
    return m_Attached;
}

HRESULT CUIElement::SetSize(SizeF Size)
{
    HRESULT hr = S_OK;

    //TODO: These should probably go through the property system.
    IFC(InternalSetWidth(Size.width));
    IFC(InternalSetHeight(Size.height));

Cleanup:
    return hr;
}

HRESULT CUIElement::Measure(SizeF AvailableSize)
{
    HRESULT hr = S_OK;

    if(m_MeasureDirty || m_LastMeasureSize.height != AvailableSize.height || m_LastMeasureSize.width != AvailableSize.width)
    {
        SizeF NewSize = { 0 };

        IFC(MeasureInternal(AvailableSize, NewSize));

        m_MeasureDirty = FALSE;
        m_LastMeasureSize = AvailableSize;

        if(NewSize.width != m_DesiredSize.width || NewSize.height != m_DesiredSize.width)
        {
            m_DesiredSize = NewSize;

            IFC(InvalidateArrange());
        }
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;

    IFCEXPECT(AvailableSize.width >= 0);
    IFCEXPECT(AvailableSize.height >= 0);

    DesiredSize.width = min(AvailableSize.width, m_Size.width);
    DesiredSize.height = min(AvailableSize.height, m_Size.height);

Cleanup:
    return hr;
}

SizeF CUIElement::GetDesiredSize()
{
    return m_DesiredSize;
}

HRESULT CUIElement::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Size.width >= 0);
    IFCEXPECT(Size.height >= 0);

    m_FinalSize = Size;

Cleanup:
    return hr;
}

HRESULT CUIElement::InvalidateMeasure()
{
    HRESULT hr = S_OK;
    CChildMeasureInvalidatedNotification* pNotification = NULL;

    if(!m_MeasureDirty)
    {
        m_MeasureDirty = TRUE;

        IFC(CChildMeasureInvalidatedNotification::Create(&pNotification));

        IFC(NotifyParent(pNotification));
    }

Cleanup:
    ReleaseObject(pNotification);

    return hr;
}

HRESULT CUIElement::InvalidateArrange()
{
    HRESULT hr = S_OK;
    CChildArrangeInvalidatedNotification* pNotification = NULL;

    if(!m_ArrangeDirty)
    {
        m_ArrangeDirty = TRUE;

        IFC(CChildArrangeInvalidatedNotification::Create(&pNotification));

        IFC(NotifyParent(pNotification));
    }

Cleanup:
    ReleaseObject(pNotification);

    return hr;
}

BOOL CUIElement::IsMeasureDirty()
{
    return m_MeasureDirty;
}

BOOL CUIElement::IsArrangeDirty()
{
    return m_ArrangeDirty;
}

CUIElement* CUIElement::GetParent()
{
    return m_Context.GetParent();
}

HRESULT CUIElement::NotifyParent(CUINotification* pNotification)
{
    HRESULT hr = S_OK;
    CUIElement* pParent = NULL;

    IFCPTR(pNotification);

    pParent = GetParent();

    if(pParent)
    {
        IFC(pParent->OnNotification(pNotification));
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::OnNotification(CUINotification* pNotification)
{
    HRESULT hr = S_OK;

    IFCPTR(pNotification);

    switch(pNotification->GetType())
    {
        case UINotification::ChildMeasureInvalidated:
            {
                CChildMeasureInvalidatedNotification* pMeasureInvalidationNotification = (CChildMeasureInvalidatedNotification*)pNotification;
                IFC(OnChildMeasureInvalidated(pMeasureInvalidationNotification));
                break;
            }

        case UINotification::ChildArrangeInvalidated:
            {
                CChildArrangeInvalidatedNotification* pArrangeInvalidationNotification = (CChildArrangeInvalidatedNotification*)pNotification;
                IFC(OnChildArrangeInvalidated(pArrangeInvalidationNotification));
                break;
            }
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::OnChildMeasureInvalidated(CChildMeasureInvalidatedNotification* pNotification)
{
    HRESULT hr = S_OK;

    IFCPTR(pNotification);

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CUIElement::OnChildArrangeInvalidated(CChildArrangeInvalidatedNotification* pNotification)
{
    HRESULT hr = S_OK;

    IFCPTR(pNotification);

    IFC(InvalidateArrange());

Cleanup:
    return hr;
}

HRESULT CUIElement::GetPropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;

    IFCPTR(ppInformation);

    if(m_PropertyInformation == NULL)
    {
        IFC(CreatePropertyInformation(&m_PropertyInformation));
    }

    *ppInformation = m_PropertyInformation;
    AddRefObject(m_PropertyInformation);

Cleanup:
    return hr;
}

HRESULT CUIElement::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(&UIElementPropertyInformation, &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

HRESULT CUIElement::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    if(wcscmp(pProperty->GetName(), L"Height") == 0)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

        CFloatValue* pFloat = (CFloatValue*)pValue;

        IFC(InternalSetHeight(pFloat->GetValue()));
    }
    else if(wcscmp(pProperty->GetName(), L"Width") == 0)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

        CFloatValue* pFloat = (CFloatValue*)pValue;

        IFC(InternalSetWidth(pFloat->GetValue()));
    }
    else
    {
        IFC(E_FAIL);
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    //if(wcscmp(pProperty->GetName(), L"Height") == 0)
    //{
    //    __debugbreak;

    //    //TODO: Implement!
    //}
    //else if(wcscmp(pProperty->GetName(), L"Width") == 0)
    //{
    //    __debugbreak;

    //    //TODO: Implement!
    //}
    //else
    {
        __debugbreak();

        IFC(E_FAIL);
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::InternalSetWidth(FLOAT Width)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Width >= 0);

    m_Size.width = Width;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}
HRESULT CUIElement::InternalSetHeight(FLOAT Height)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Height >= 0);

    m_Size.height = Height;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}