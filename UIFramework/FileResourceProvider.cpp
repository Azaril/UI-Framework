#include "FileResourceProvider.h"
#include "FileResourceStream.h"

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

    return hr;
}

__override __checkReturn HRESULT
CFileResourceProvider::ReadResource(
    __in_ecount(identiferLength) WCHAR* pIdentifier,
    UINT32 identifierLength,
    __deref_out IReadStream** ppStream
    )
{
    HRESULT hr = S_OK;
    FILE* pFile = NULL;
    CFileResourceStream* pFileStream = NULL;

    pFile = _wfsopen(pIdentifier, L"rb", _SH_DENYWR);
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