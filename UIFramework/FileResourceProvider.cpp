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

#ifdef _WINDOWS
    pFile = _wfsopen(pIdentifier, L"rb", _SH_DENYWR);
    IFCPTR(pFile);
#else
    {
        StackHeapBuffer<CHAR, 2048> stringBuffer;
        
        hr = ConvertWCHARToUTF8< CHAR, 2048 >(pIdentifier, &stringBuffer, NULL);
        IFC(hr);
        
        pFile = fopen(stringBuffer.GetBuffer(), "rb");
        IFCPTR(pFile);
    }
#endif

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