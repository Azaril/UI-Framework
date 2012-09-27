#include "FileResourceProvider.h"
#include "FileResourceStream.h"

#ifndef _WINDOWS
#include "StringConversion.h"
#endif

CFileResourceProvider::CFileResourceProvider(
    )
{
}

CFileResourceProvider::~CFileResourceProvider(
    )
{
}

__checkReturn HRESULT 
CFileResourceProvider::Initialize(
    )
{
    HRESULT hr = S_OK;

    m_SearchPaths.push_back(L"");

    return hr;
}

__checkReturn HRESULT 
CFileResourceProvider::AddSearchPath(
    __in_ecount(PathLength) const WCHAR* pPath,
    UINT32 PathLength
    )
{
    HRESULT hr = S_OK;

    m_SearchPaths.push_back(std::wstring(pPath, PathLength));

    return hr;
}

__override __checkReturn HRESULT
CFileResourceProvider::ReadResource(
    __in_ecount(identiferLength) const WCHAR* pIdentifier,
    UINT32 identifierLength,
    __deref_out IReadStream** ppStream
    )
{
    HRESULT hr = S_OK;
    CFileResourceStream* pFileStream = NULL;

    for (list< std::wstring >::iterator it = m_SearchPaths.begin(); it != m_SearchPaths.end(); ++it)
    {
        std::wstring combinedPath = *it;

        if (!combinedPath.empty())
        {
#if defined(_WINDOWS) || defined(_XBOX)
			const WCHAR separator = L'\\';
#else
            const WCHAR separator = L'/';
#endif

			if (combinedPath[combinedPath.length() - 1] != separator)
			{
				combinedPath.append(&separator, 1);
			}
        }

        combinedPath.append(pIdentifier, identifierLength);

        if (SUCCEEDED(CFileResourceStream::CreateOnPath(combinedPath.c_str(), &pFileStream)))
        {
            break;
        }
    }

    IFCPTR(pFileStream);

    *ppStream = pFileStream;
    pFileStream = NULL;

Cleanup:
    ReleaseObject(pFileStream);

    return hr;
}