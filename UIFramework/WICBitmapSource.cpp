#include "WICBitmapSource.h"

CWICBitmapSource::CWICBitmapSource() : m_Source(NULL)
{
}

CWICBitmapSource::~CWICBitmapSource()
{
    ReleaseObject(m_Source);
}

HRESULT CWICBitmapSource::Initialize(IWICBitmapSource* pSource)
{
    HRESULT hr = S_OK;

    IFCPTR(pSource);

    m_Source = pSource;
    AddRefObject(m_Source);

Cleanup:
    return hr;
}

IWICBitmapSource* CWICBitmapSource::GetWICBitmapSource()
{
    return m_Source;
}

HRESULT CWICBitmapSource::GetSize(SizeU* pSize)
{
    HRESULT hr = S_OK;

    IFCPTR(pSize);

    IFC(m_Source->GetSize(&pSize->width, &pSize->height));

Cleanup:
    return hr;
}