#pragma once

#include "ReadStream.h"
#include "COMRefCounted.h"
#include "Factory.h"

#include <dwrite.h>

class CDirectWriteFontFileStream : public CCOMRefCountedObjectBase< IDWriteFontFileStream >
{
    public:
        DECLARE_FACTORY1( CDirectWriteFontFileStream, IReadStream* );

        //
        // IUnknown
        //
        STDMETHOD(QueryInterface)( 
            REFIID riid,
            __deref_out void** ppvObject
            );

        //
        // IDWriteFontFileStream
        //
        STDMETHOD(ReadFileFragment)(
            __deref_out_bcount(fragmentSize) void const** ppFragmentStart,
            UINT64 fileOffset,
            UINT64 fragmentSize,
            __out void** ppFragmentContext
            );

        STDMETHOD_(void, ReleaseFileFragment)(
            void* pFragmentContext
            );

        STDMETHOD(GetFileSize)(
            __out UINT64* pFileSize
            );

        STDMETHOD(GetLastWriteTime)(
            __out UINT64* pLastWriteTime
            );

    protected:
        CDirectWriteFontFileStream(
            );

        virtual ~CDirectWriteFontFileStream(
            );

        __checkReturn HRESULT Initialize(
            __in IReadStream* pStream
            );

        IReadStream* m_pStream;
        UINT64 m_StreamSize;
        CRITICAL_SECTION m_Lock;
};

