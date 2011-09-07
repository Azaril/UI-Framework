#include "LayoutManager.h"

CLayoutManager::CLayoutManager(
    ) 
    : m_RootElement(NULL)
    , m_LayoutSize(0, 0)
{
}

CLayoutManager::~CLayoutManager(
    )
{
    ReleaseObject(m_RootElement);
}

__checkReturn HRESULT
CLayoutManager::Initialize(
    __in CUIElement* pRootElement
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRootElement);

    m_RootElement = pRootElement;
    AddRefObject(m_RootElement);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CLayoutManager::SetLayoutSize(
    const SizeF& LayoutSize
    )
{
    HRESULT hr = S_OK;

    if(m_LayoutSize != LayoutSize)
    {
        m_LayoutSize = LayoutSize;

        IFC(m_RootElement->InvalidateMeasure());
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CLayoutManager::EnsureLayout(
    )
{
    HRESULT hr = S_OK;

    for(UINT32 i = 0; i < 10 && m_RootElement->IsMeasureDirty(); i++)
    {
        IFC(m_RootElement->Measure(m_LayoutSize));
    }

    IFCEXPECT(!m_RootElement->IsMeasureDirty());

    for(UINT32 i = 0; i < 10 && m_RootElement->IsArrangeDirty(); i++)
    {
        IFC(m_RootElement->Arrange(MakeRect(m_LayoutSize)));
    }

    IFCEXPECT(!m_RootElement->IsArrangeDirty());

Cleanup:
    return hr;
}