#include "CompositeResourceProvider.h"

CCompositeResourceProvider::CCompositeResourceProvider(
    )
{
}

CCompositeResourceProvider::~CCompositeResourceProvider(
    )
{
    for (vector< IResourceProvider* >::iterator it = m_ResourceProviders.begin(); it != m_ResourceProviders.end(); ++it)
    {
        (*it)->Release();
    }
}

__checkReturn HRESULT
CCompositeResourceProvider::Initialize(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT
CCompositeResourceProvider::AddResourceProvider(
    __in IResourceProvider* pProvider
    )
{
    HRESULT hr = S_OK;

    m_ResourceProviders.push_back(pProvider);

    AddRefObject(pProvider);

    return hr;
}

__checkReturn HRESULT
CCompositeResourceProvider::ReadResource(
    __in_ecount(identiferLength) WCHAR* pIdentifier,
    UINT32 identifierLength,
    __deref_out IReadStream** ppStream
    )
{
    HRESULT hr = S_OK;

    for (vector< IResourceProvider* >::iterator it = m_ResourceProviders.begin(); it != m_ResourceProviders.end(); ++it)
    {
        if (SUCCEEDED((*it)->ReadResource(pIdentifier, identifierLength, ppStream)))
        {
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}