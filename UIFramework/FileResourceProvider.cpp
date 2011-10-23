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
    FILE* pFile = NULL;
    CFileResourceStream* pFileStream = NULL;

    for (list< std::wstring >::iterator it = m_SearchPaths.begin(); pFile == NULL && it != m_SearchPaths.end(); ++it)
    {
        std::wstring combinedPath = *it;

        if (!combinedPath.empty())
        {
            combinedPath.append(L"/");
        }

        combinedPath.append(pIdentifier, identifierLength);

#ifdef _WINDOWS
        pFile = _wfsopen(combinedPath.c_str(), L"rb", _SH_DENYWR);
#else
        {
            StackHeapBuffer<CHAR, 2048> stringBuffer;

            hr = ConvertWCHARToUTF8< CHAR, 2048 >(combinedPath.c_str(), combinedPath.length(), &stringBuffer, NULL);
            IFC(hr);

            pFile = fopen(stringBuffer.GetBuffer(), "rb");
            IFCPTR(pFile);
        }
#endif
    }

    IFCPTR(pFile);

    IFC(CFileResourceStream::Create(pFile, &pFileStream));

    pFile = NULL;

    *ppStream = pFileStream;
    pFileStream = NULL;

Cleanup:
    ReleaseObject(pFileStream);

    if (pFile != NULL)
    {
        fclose(pFile);
    }

    return hr;
}