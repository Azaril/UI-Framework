#include "COMReadStreamWrapper.h"
#include "AutoLock.h"

#if defined(FRAMEWORK_XMLLITE) || defined(FRAMEWORK_WIC)

CCOMReadStreamWrapper::CCOMReadStreamWrapper( 
    )
    : m_pStream(NULL)
{
}

CCOMReadStreamWrapper::~CCOMReadStreamWrapper(
    )
{
    ReleaseObject(m_pStream);

    DeleteCriticalSection(&m_Lock);
}

__checkReturn HRESULT
CCOMReadStreamWrapper::Initialize(
    __in IReadStream* pStream
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pStream, pStream);

    InitializeCriticalSection(&m_Lock);

    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::QueryInterface(
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
    else if (riid == __uuidof(ISequentialStream))
    {
        SetObject(*ppvObject, (ISequentialStream*)this);
    }
    else if (riid == __uuidof(IStream))
    {
        SetObject(*ppvObject, (IStream*)this);
    }
    else
    {
        IFC_NOTRACE(E_NOINTERFACE);
    }

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::Read(
    __out_bcount_part(cb, *pcbRead) void* pv,
    ULONG cb,
    __out_opt ULONG* pcbRead
    )
{
    HRESULT hr = S_OK;
    UINT64 bytesRead = 0;

    CAutoLock lock(&m_Lock);

    IFC(m_pStream->Read(pv, cb, &bytesRead));

    if (pcbRead != NULL)
    {
        *pcbRead = (ULONG)bytesRead;
    }

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::Write(
    __in_bcount(cb) const void* pv,
    ULONG cb,
    __out_opt  ULONG *pcbWritten
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::Seek(
    LARGE_INTEGER dlibMove,
    DWORD dwOrigin,
    __out_opt ULARGE_INTEGER* plibNewPosition
    )
{
    HRESULT hr = S_OK;
    SeekType::Value seekType = SeekType::Begin;

    CAutoLock lock(&m_Lock);

    switch (dwOrigin)
    {
        case STREAM_SEEK_SET:
            {
                seekType = SeekType::Begin;

                break;
            }

        case STREAM_SEEK_CUR:
            {
                seekType = SeekType::Current;

                break;
            }

        case STREAM_SEEK_END:
            {
                seekType = SeekType::End;

                break;
            }

        default: 
            {
                IFC(E_UNEXPECTED);
            }
    }

    IFC(m_pStream->Seek(seekType, dlibMove.QuadPart, (plibNewPosition != NULL) ? &plibNewPosition->QuadPart : NULL));

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::SetSize( 
    ULARGE_INTEGER libNewSize
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::CopyTo( 
    IStream *pstm,
    ULARGE_INTEGER cb,
    __out_opt ULARGE_INTEGER* pcbRead,
    __out_opt  ULARGE_INTEGER* pcbWritten
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::Commit( 
    DWORD grfCommitFlags
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::Revert(
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::LockRegion( 
    ULARGE_INTEGER libOffset,
    ULARGE_INTEGER cb,
    DWORD dwLockType
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::UnlockRegion( 
    ULARGE_INTEGER libOffset,
    ULARGE_INTEGER cb,
    DWORD dwLockType
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::Stat( 
    STATSTG* pstatstg,
    DWORD grfStatFlag
    )
{
    HRESULT hr = S_OK;

    CAutoLock lock(&m_Lock);

    ZeroMemory(pstatstg, sizeof(STATSTG));

    IFCEXPECT(grfStatFlag & STATFLAG_NONAME);

    pstatstg->type = STGTY_STREAM;

    IFC(m_pStream->GetSize(&pstatstg->cbSize.QuadPart));

Cleanup:
    return hr;
}

STDMETHODIMP
CCOMReadStreamWrapper::Clone( 
    IStream** ppstm
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

#endif