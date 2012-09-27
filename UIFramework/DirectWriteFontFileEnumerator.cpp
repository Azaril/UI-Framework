#include "DirectWriteFontFileEnumerator.h"

#if defined(FRAMEWORK_DWRITE)

CDirectWriteFontFileEnumerator::CDirectWriteFontFileEnumerator(
    )
    : m_Current(-1)
    , m_pFontTokens(NULL)
    , m_TokenCount(0)
    , m_pCurrentFontFile(NULL)
    , m_pFontFileLoader(NULL)
{
}

CDirectWriteFontFileEnumerator::~CDirectWriteFontFileEnumerator(
    )
{
    delete [] m_pFontTokens;

    ReleaseObject(m_pCurrentFontFile);
    ReleaseObject(m_pFontFileLoader);
}

__checkReturn HRESULT
CDirectWriteFontFileEnumerator::Initialize(
    __in IDWriteFactory* pFactory,
    __in CDirectWriteFontFileLoader* pFileLoader,
    __in_ecount(tokenCount) UINT32* pFontTokens,
    UINT32 tokenCount
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pFactory, pFactory);
    SetObject(m_pFontFileLoader, pFileLoader);

    m_pFontTokens = new UINT32[tokenCount];
    IFCOOM(m_pFontTokens);

    m_TokenCount = tokenCount;

    memcpy(m_pFontTokens, pFontTokens, sizeof(UINT32) * m_TokenCount);

Cleanup:
    return hr;
}

STDMETHODIMP
CDirectWriteFontFileEnumerator::QueryInterface(
    REFIID riid,
    __deref_out void** ppvObject
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppvObject);

    *ppvObject = NULL;

    if (riid == __uuidof(IUnknown))
    {
        SetObject(*ppvObject, (IUnknown*)this);
    }
    else if (riid == __uuidof(IDWriteFontFileEnumerator))
    {
        SetObject(*ppvObject, (IDWriteFontFileEnumerator*)this);
    }
    else
    {
        IFC_NOTRACE(E_NOINTERFACE);
    }

Cleanup:
    return hr;
}

STDMETHODIMP
CDirectWriteFontFileEnumerator::MoveNext(
    __out BOOL* pHasCurrentFile
    )
{
    HRESULT hr = S_OK;

    if (m_Current < m_TokenCount)
    {
        ++m_Current;
    }

    IFCEXPECT(m_Current >= 0 && m_Current <= m_TokenCount);

    ReleaseObject(m_pCurrentFontFile);

    if (m_Current < m_TokenCount)
    {
        IFC(m_pFactory->CreateCustomFontFileReference(&m_pFontTokens[m_Current], sizeof(m_pFontTokens[m_Current]), m_pFontFileLoader, &m_pCurrentFontFile));
    }

    *pHasCurrentFile = (m_pCurrentFontFile != NULL);

Cleanup:
    return hr;
}

STDMETHODIMP
CDirectWriteFontFileEnumerator::GetCurrentFontFile(
    __out IDWriteFontFile** ppFontFile
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_pCurrentFontFile != NULL);

    SetObject(*ppFontFile, m_pCurrentFontFile);

Cleanup:
    return hr;
}

#endif