#include "FileResourceStream.h"

CFileResourceStream::CFileResourceStream(
    )
    : m_pFile(NULL)
    , m_FileSize(0)
{
}

CFileResourceStream::~CFileResourceStream(
    )
{
    if (m_pFile != NULL)
    {
        fclose(m_pFile);
    }
}

__checkReturn HRESULT
CFileResourceStream::Initialize(
    __in FILE* pFile
    )
{
    HRESULT hr = S_OK;

    m_pFile = pFile;

    IFCEXPECT(_fseeki64(m_pFile, 0, SEEK_END) == 0);

    m_FileSize = _ftelli64(m_pFile);

    IFCEXPECT(_fseeki64(m_pFile, 0, SEEK_SET) == 0);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CFileResourceStream::GetSize( 
    __out UINT64* pSize 
    )
{
    HRESULT hr = S_OK;

    *pSize = m_FileSize;

    return hr;
}

__checkReturn HRESULT
CFileResourceStream::Seek(
    SeekType::Value seekType,
    INT64 position,
    __out_opt UINT64* pNewPosition
    )
{
    HRESULT hr = S_OK;
    INT32 seekParam = SEEK_SET;

    switch (seekType)
    {
        case SeekType::Begin:
            {
                seekParam = SEEK_SET;
                break;
            }

        case SeekType::Current:
            {
                seekParam = SEEK_CUR;
                break;
            }

        case SeekType::End:
            {
                seekParam = SEEK_END;
                break;
            }

        default:
            {
                IFC(E_UNEXPECTED);
            }
    }

    IFCEXPECT(_fseeki64(m_pFile, position, seekParam) == 0);

    if (pNewPosition != NULL)
    {
        *pNewPosition = _ftelli64(m_pFile);
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFileResourceStream::Read(
    __out_bcount_part(bufferSize, *pBytesWritten) void* pBuffer,
    UINT64 bufferSize,
    __out UINT64* pBytesWritten
    )
{
    HRESULT hr = S_OK;
    size_t readResult = 0;

    readResult = fread(pBuffer, sizeof(BYTE), (size_t)bufferSize, m_pFile);
    
    IFCEXPECT(ferror(m_pFile) == 0);

    *pBytesWritten = readResult;

Cleanup:
    return hr;
}