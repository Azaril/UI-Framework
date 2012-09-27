#include "DirectWriteFontFileLoader.h"
#include "DirectWriteFontFileStream.h"

#if defined(FRAMEWORK_DWRITE)

CDirectWriteFontFileLoader::CDirectWriteFontFileLoader(
    )
    : m_pFontCollection(NULL)
{
}

CDirectWriteFontFileLoader::~CDirectWriteFontFileLoader(
    )
{
    ReleaseObject(m_pFontCollection);
}

__checkReturn HRESULT 
CDirectWriteFontFileLoader::Initialize(
    __in CDirectWriteRegisteredFontCollection* pFontCollection
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pFontCollection, pFontCollection);

    return hr;
}

STDMETHODIMP
CDirectWriteFontFileLoader::QueryInterface(
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
    else if (riid == __uuidof(IDWriteFontFileLoader))
    {
        SetObject(*ppvObject, (IDWriteFontFileLoader*)this);
    }
    else
    {
        IFC_NOTRACE(E_NOINTERFACE);
    }

Cleanup:
    return hr;
}

STDMETHODIMP
CDirectWriteFontFileLoader::CreateStreamFromKey(
    __in_bcount(fontFileReferenceKeySize) void const* pFontFileReferenceKey,
    UINT32 fontFileReferenceKeySize,
    __deref_out IDWriteFontFileStream** ppFontFileStream
    )
{
    HRESULT hr = S_OK;
    UINT32 fontToken = 0;
    IReadStream* pReadStream = NULL;
    CDirectWriteFontFileStream* pFontFileStream = NULL;

    IFCEXPECT(fontFileReferenceKeySize == sizeof(fontToken));

    fontToken = *(UINT32*)pFontFileReferenceKey;

    IFC(m_pFontCollection->GetReadStream(fontToken, &pReadStream));

    IFC(CDirectWriteFontFileStream::Create(pReadStream, &pFontFileStream));

    *ppFontFileStream = pFontFileStream;
    pFontFileStream = NULL;

Cleanup:
    ReleaseObject(pReadStream);
    ReleaseObject(pFontFileStream);

    return hr;
}

#endif