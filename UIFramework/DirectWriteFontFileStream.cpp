#include "DirectWriteFontFileStream.h"
#include "AutoLock.h"

CDirectWriteFontFileStream::CDirectWriteFontFileStream(
    )
    : m_pStream(NULL)
    , m_StreamSize(0)
{
}

CDirectWriteFontFileStream::~CDirectWriteFontFileStream(
    )
{
    ReleaseObject(m_pStream);

    DeleteCriticalSection(&m_Lock);
}

__checkReturn HRESULT
CDirectWriteFontFileStream::Initialize(
    __in IReadStream* pStream
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pStream, pStream);

    IFC(m_pStream->GetSize(&m_StreamSize));

    InitializeCriticalSection(&m_Lock);

Cleanup:
    return hr;
}

STDMETHODIMP
CDirectWriteFontFileStream::QueryInterface(
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
    else if (riid == __uuidof(IDWriteFontFileStream))
    {
        SetObject(*ppvObject, (IDWriteFontFileStream*)this);
    }
    else
    {
        IFC_NOTRACE(E_NOINTERFACE);
    }

Cleanup:
    return hr;
}

STDMETHODIMP
CDirectWriteFontFileStream::ReadFileFragment(
    __deref_out_bcount(fragmentSize) void const** ppFragmentStart,
    UINT64 fileOffset,
    UINT64 fragmentSize,
    __out void** ppFragmentContext
    )
{
    HRESULT hr = S_OK;
    BYTE* pBuffer = NULL;
    UINT64 bytesRead = 0;

    //TODO: Pool buffers.

    IFCEXPECT(fileOffset + fragmentSize <= m_StreamSize);

    //
    // DWrite can call this method from multiple threads so to ensure
    // the correct stream state is maintained a lock is required.
    //
    {
        CAutoLock lock(&m_Lock);

        IFC(m_pStream->Seek(SeekType::Begin, fileOffset, NULL));

        pBuffer = new BYTE[(size_t)fragmentSize];
        IFCOOM(pBuffer);

        IFC(m_pStream->Read(pBuffer, fragmentSize, &bytesRead));
    }

    IFCEXPECT(bytesRead == fragmentSize);

    *ppFragmentStart = pBuffer;
    *ppFragmentContext = pBuffer;

    pBuffer = NULL;

Cleanup:
    delete [] pBuffer;

    return hr;
}

STDMETHODIMP_(void)
CDirectWriteFontFileStream::ReleaseFileFragment(
    void* pFragmentContext
    )
{
    BYTE* pBuffer = (BYTE*)pFragmentContext;

    delete [] pBuffer;
}

STDMETHODIMP
CDirectWriteFontFileStream::GetFileSize(
    __out UINT64* pFileSize
    )
{
    HRESULT hr = S_OK;

    *pFileSize = m_StreamSize;

    return hr;
}

STDMETHODIMP
CDirectWriteFontFileStream::GetLastWriteTime(
    __out UINT64* pLastWriteTime
    )
{
    HRESULT hr = S_OK;

    //
    // Not implemented as this could be any stream source with no time stamp.
    //
    IFC_NOTRACE(E_NOTIMPL);

Cleanup:
    return hr;
}