#include "UIElement.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "DelegatingPropertyInformation.h"

CStaticProperty UIElementProperties[] = 
{
    CStaticProperty( L"Width", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"Height", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"Visibility", TypeIndex::Visibility, StaticPropertyFlags::None )
};

namespace UIElementPropertyIndex
{
    enum Value
    {
        Width,
        Height,
        Visibility
    };
}

CUIElement::CUIElement() : m_Attached(FALSE),
                           m_MeasureDirty(TRUE),
                           m_ArrangeDirty(TRUE),
                           m_PropertyInformation(NULL),
                           m_Visibility(Visibility::Visible)
{
    m_Size.width = 0;
    m_Size.height = 0;

    m_DesiredSize.width = 0;
    m_DesiredSize.height = 0;

    m_LastMeasureSize.width = 0;
    m_LastMeasureSize.height = 0;

    m_FinalSize.width = 0;
    m_FinalSize.height = 0;
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

HRESULT CUIElement::PreRender(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    if(m_Visibility == Visibility::Visible)
    {
        IFC(PreRenderInternal(Context));
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CVisual::PreRender(Context));

Cleanup:
    return hr;
}

HRESULT CUIElement::Render(CRenderContext& Context)
{
    HRESULT hr = S_OK;

    if(m_Visibility == Visibility::Visible)
    {
        IFC(RenderInternal(Context));
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::RenderInternal(CRenderContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CVisual::Render(Context));

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
        if(m_Visibility == Visibility::Visible || m_Visibility == Visibility::Hidden)
        {
            SizeF NewSize = { 0 };

            IFC(MeasureInternal(AvailableSize, NewSize));

            m_LastMeasureSize = AvailableSize;

            if(NewSize.width != m_DesiredSize.width || NewSize.height != m_DesiredSize.width)
            {
                m_DesiredSize = NewSize;

                IFC(InvalidateArrange());
            }
        }
        else
        {
            m_DesiredSize.width = 0;
            m_DesiredSize.height = 0;
        }

        m_MeasureDirty = FALSE;
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

SizeF CUIElement::GetFinalSize()
{
    return m_FinalSize;
}

HRESULT CUIElement::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Size.width >= 0);
    IFCEXPECT(Size.height >= 0);

    if(m_ArrangeDirty || Size.width != m_FinalSize.width || Size.height != m_FinalSize.width)
    {
        if(m_Visibility == Visibility::Visible || m_Visibility == Visibility::Hidden)
        {
            m_FinalSize = Size;

            IFC(ArrangeInternal(Size));
        }
        else
        {
            m_FinalSize.width = 0;
            m_FinalSize.height = 0;
        }

        m_ArrangeDirty = FALSE;
    }

Cleanup:
    return hr;
}

HRESULT CUIElement::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;

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

HRESULT CUIElement::SetVisibility(Visibility::Value State)
{
    HRESULT hr = S_OK;

    IFC(InternalSetVisibility(State));

Cleanup:
    return hr;
}

HRESULT CUIElement::InternalSetVisibility(Visibility::Value State)
{
    HRESULT hr = S_OK;

    IFCEXPECT(State == Visibility::Visible || State == Visibility::Hidden || State == Visibility::Collapsed);

    m_Visibility = State;

    IFC(InvalidateMeasure());
    IFC(InvalidateArrange());

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

HRESULT CUIElement::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(UIElementProperties, ARRAYSIZE(UIElementProperties), &pStaticInformation))
    IFC(CVisual::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CUIElement::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= UIElementProperties && pProperty < UIElementProperties + ARRAYSIZE(UIElementProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - UIElementProperties);
        
        switch(Index)
        {
            case UIElementPropertyIndex::Width:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(InternalSetHeight(pFloat->GetValue()));

                    break;
                }

            case UIElementPropertyIndex::Height:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(InternalSetWidth(pFloat->GetValue()));

                    break;
                }

            case UIElementPropertyIndex::Visibility:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Visibility));

                    CVisibilityValue* pVisibility = (CVisibilityValue*)pValue;

                    IFC(InternalSetVisibility(pVisibility->GetValue()));

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
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
    CFloatValue* pFloatValue = NULL;
    CVisibilityValue* pVisibilityValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    // Check if the property is a static property.
    if(pProperty >= UIElementProperties && pProperty < UIElementProperties + ARRAYSIZE(UIElementProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - UIElementProperties);
        
        switch(Index)
        {
            case UIElementPropertyIndex::Width:
                {
                    IFC(CFloatValue::Create(m_Size.width, &pFloatValue));

                    *ppValue = pFloatValue;
                    pFloatValue = NULL;

                    break;
                }

            case UIElementPropertyIndex::Height:
                {
                    IFC(CFloatValue::Create(m_Size.width, &pFloatValue));

                    *ppValue = pFloatValue;
                    pFloatValue = NULL;

                    break;
                }

            case UIElementPropertyIndex::Visibility:
                {
                    IFC(CVisibilityValue::Create(m_Visibility, &pVisibilityValue));

                    *ppValue = pVisibilityValue;
                    pVisibilityValue = NULL;

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(E_FAIL);
    }

Cleanup:
    ReleaseObject(pFloatValue);
    ReleaseObject(pVisibilityValue);

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