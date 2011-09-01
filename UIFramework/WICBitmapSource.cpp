#include "WICBitmapSource.h"

CWICBitmapSource::CWICBitmapSource(
	) : m_Source(NULL)
	, m_Stream(NULL)
{
}

CWICBitmapSource::~CWICBitmapSource(
	)
{
    ReleaseObject(m_Source);
    ReleaseObject(m_Stream);
}

__checkReturn HRESULT 
CWICBitmapSource::Initialize(
	__in IWICBitmapSource* pSource
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pSource);

    m_Source = pSource;
    AddRefObject(m_Source);

Cleanup:
    return hr;
}

__out IWICBitmapSource* 
CWICBitmapSource::GetWICBitmapSource(
	)
{
    return m_Source;
}

__override __checkReturn HRESULT 
CWICBitmapSource::GetSize(
	__out SizeU* pSize
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pSize);

    IFC(m_Source->GetSize(&pSize->width, &pSize->height));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CWICBitmapSource::AssociateStream(
	__in IStream* pStream
	)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_Stream);

    m_Stream = pStream;

    AddRefObject(m_Stream);

    return hr;
}