#pragma once

#if defined(FRAMEWORK_XMLLITE) || defined(FRAMEWORK_WIC)

#include "ReadStream.h"
#include "Factory.h"
#include "COMRefCounted.h"

class CCOMReadStreamWrapper : public CCOMRefCountedObjectBase< IStream >
{
    public:
        DECLARE_FACTORY1( CCOMReadStreamWrapper, IReadStream* );

        //
        // IUnknown
        //
        STDMETHOD(QueryInterface)( 
            REFIID riid,
            __deref_out void** ppvObject
            );

        //
        // ISequentialStream
        //
        STDMETHOD(Read)( 
            __out_bcount_part(cb, *pcbRead) void* pv,
            ULONG cb,
            __out_opt ULONG* pcbRead
            );

        STDMETHOD(Write)( 
            __in_bcount(cb) const void* pv,
            ULONG cb,
            __out_opt  ULONG *pcbWritten
            );

        //
        // IStream
        //
        STDMETHOD(Seek)(
            LARGE_INTEGER dlibMove,
            DWORD dwOrigin,
            __out_opt ULARGE_INTEGER* plibNewPosition
            );

        STDMETHOD(SetSize)( 
            ULARGE_INTEGER libNewSize
            );

        STDMETHOD(CopyTo)( 
            IStream *pstm,
            ULARGE_INTEGER cb,
            __out_opt ULARGE_INTEGER* pcbRead,
            __out_opt  ULARGE_INTEGER* pcbWritten
            );

        STDMETHOD(Commit)( 
            DWORD grfCommitFlags
            );

        STDMETHOD(Revert)(
            );

        STDMETHOD(LockRegion)( 
            ULARGE_INTEGER libOffset,
            ULARGE_INTEGER cb,
            DWORD dwLockType
            );

        STDMETHOD(UnlockRegion)( 
            ULARGE_INTEGER libOffset,
            ULARGE_INTEGER cb,
            DWORD dwLockType
            );

        STDMETHOD(Stat)( 
            STATSTG* pstatstg,
            DWORD grfStatFlag
            );

        STDMETHOD(Clone)( 
            IStream** ppstm
            );

    protected:
        CCOMReadStreamWrapper(
            );

        virtual ~CCOMReadStreamWrapper(
            );

        __checkReturn HRESULT Initialize(
            __in IReadStream* pStream
            );

        IReadStream* m_pStream;
        CRITICAL_SECTION m_Lock;
};

#endif