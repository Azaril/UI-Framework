#include "UIElement.h"

CUIElement::CUIElement() : m_Attached(FALSE)
{
    m_Size.width = 0;
    m_Size.height = 0;

    m_DesiredSize.width = 0;
    m_DesiredSize.height = 0;
}

CUIElement::~CUIElement()
{
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

    IFCEXPECT(Size.width >= 0);
    IFCEXPECT(Size.height >= 0);

    m_Size = Size;

Cleanup:
    return hr;
}

HRESULT CUIElement::Measure(SizeF AvailableSize)
{
    HRESULT hr = S_OK;

    IFC(MeasureInternal(AvailableSize, m_DesiredSize));

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