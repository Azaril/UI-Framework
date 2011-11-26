#include "DirectWriteRegisteredFontCollection.h"

CDirectWriteRegisteredFontCollection::CDirectWriteRegisteredFontCollection(
    )
{
}

CDirectWriteRegisteredFontCollection::~CDirectWriteRegisteredFontCollection(
    )
{
}

__checkReturn HRESULT
CDirectWriteRegisteredFontCollection::Initialize(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT
CDirectWriteRegisteredFontCollection::RegisterFont(
    __in IResourceProvider* pResourceProvider,
    __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
    UINT32 IdentifierLength,
    __out UINT32* pFontToken
    )
{
    HRESULT hr = S_OK;

    m_RegisteredFonts.push_back(CFontInfo(pResourceProvider, pIdentifier, IdentifierLength));

    *pFontToken = m_RegisteredFonts.size() - 1;

    return hr;
}

__checkReturn HRESULT
CDirectWriteRegisteredFontCollection::GetReadStream(
    UINT32 fontToken,
    __deref_out IReadStream** ppStream
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(fontToken < m_RegisteredFonts.size());

    {
        CFontInfo& info = m_RegisteredFonts[fontToken];

        std::wstring identifier = info.GetIdentifier();

        IFC(info.GetResourceProvider()->ReadResource(identifier.c_str(), identifier.length(), ppStream));
    }

Cleanup:
    return hr;
}