#include "DirectWriteFontCollectionLoader.h"
#include "DirectWriteFontFileEnumerator.h"

#if defined(FRAMEWORK_DWRITE)

CDirectWriteFontCollectionLoader::CDirectWriteFontCollectionLoader(
    )
    : m_pFontFileLoader(NULL)
{
}

CDirectWriteFontCollectionLoader::~CDirectWriteFontCollectionLoader(
    )
{
    ReleaseObject(m_pFontFileLoader);
}

__checkReturn HRESULT
CDirectWriteFontCollectionLoader::Initialize(
    __in CDirectWriteFontFileLoader* pFileLoader
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pFontFileLoader, pFileLoader);

    return hr;
}

STDMETHODIMP
CDirectWriteFontCollectionLoader::QueryInterface(
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
    else if (riid == __uuidof(IDWriteFontCollectionLoader))
    {
        SetObject(*ppvObject, (IDWriteFontCollectionLoader*)this);
    }
    else
    {
        IFC_NOTRACE(E_NOINTERFACE);
    }

Cleanup:
    return hr;
}

STDMETHODIMP
CDirectWriteFontCollectionLoader::CreateEnumeratorFromKey(
    IDWriteFactory* pFactory,
    __in_bcount(collectionKeySize) void const* pCollectionKey,
    UINT32 collectionKeySize,
    __deref_out IDWriteFontFileEnumerator** ppFontFileEnumerator
    )
{
    HRESULT hr = S_OK;
    UINT32 fontToken = 0;
    CDirectWriteFontFileEnumerator* pFontEnumerator = NULL;

    IFCEXPECT(collectionKeySize == sizeof(fontToken));

    fontToken = *(UINT32*)pCollectionKey;

    IFC(CDirectWriteFontFileEnumerator::Create(pFactory, m_pFontFileLoader, &fontToken, 1, &pFontEnumerator));

    *ppFontFileEnumerator = pFontEnumerator;
    pFontEnumerator = NULL;

Cleanup:
    ReleaseObject(pFontEnumerator);

    return hr;
}

#endif