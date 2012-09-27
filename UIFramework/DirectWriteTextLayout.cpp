#include "DirectWriteTextLayout.h"
#include "DirectWriteLayoutMetrics.h"

#if defined(FRAMEWORK_DWRITE)

CDirectWriteTextLayout::CDirectWriteTextLayout(
	) 
	: m_Layout(NULL)
{
}

CDirectWriteTextLayout::~CDirectWriteTextLayout(
	)
{
    ReleaseObject(m_Layout);
}

__checkReturn HRESULT 
CDirectWriteTextLayout::Initialize(
	__in IDWriteTextLayout* pLayout
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pLayout);

    m_Layout = pLayout;
    AddRefObject(m_Layout);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CDirectWriteTextLayout::GetDirectWriteTextLayout(
	__deref_out IDWriteTextLayout** ppLayout
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppLayout);

    *ppLayout = m_Layout;
    AddRefObject(m_Layout);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteTextLayout::SetMaxSize(
	const SizeF& Size
	)
{
    HRESULT hr = S_OK;

    IFC(m_Layout->SetMaxWidth(Size.width));
    IFC(m_Layout->SetMaxHeight(Size.height));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteTextLayout::GetMetrics(
	__deref_out CTextLayoutMetrics** ppMetrics
	)
{
    HRESULT hr = S_OK;
    CDirectWriteLayoutMetrics* pDirectWriteMetrics = NULL;
    DWRITE_TEXT_METRICS Metrics;

    IFCPTR(ppMetrics);

    IFC(m_Layout->GetMetrics(&Metrics));

    IFC(CDirectWriteLayoutMetrics::Create(Metrics, &pDirectWriteMetrics));

    *ppMetrics = pDirectWriteMetrics;
    pDirectWriteMetrics = NULL;

Cleanup:
    ReleaseObject(pDirectWriteMetrics);

    return hr;
}

#endif