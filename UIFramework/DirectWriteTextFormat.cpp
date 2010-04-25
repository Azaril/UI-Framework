#include "DirectWriteTextFormat.h"
#include "ErrorChecking.h"

CDirectWriteTextFormat::CDirectWriteTextFormat() : m_Format(NULL)
{
}

CDirectWriteTextFormat::~CDirectWriteTextFormat()
{
    ReleaseObject(m_Format);
}

HRESULT CDirectWriteTextFormat::Initialize(IDWriteTextFormat* pFormat)
{
    HRESULT hr = S_OK;

    IFCPTR(pFormat);

    m_Format = pFormat;
    AddRefObject(m_Format);

Cleanup:
    return hr;
}

IDWriteTextFormat* CDirectWriteTextFormat::GetDirectWriteTextFormat()
{
    return m_Format;
}