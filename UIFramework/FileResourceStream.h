#pragma once

#include "ReadStream.h"
#include "RefCounted.h"
#include "Factory.h"

class CFileResourceStream : public CRefCountedObjectBase< IReadStream >
{
    public:
        DECLARE_FACTORY1( CFileResourceStream, FILE* );

        __override virtual __checkReturn HRESULT GetSize(
            __out UINT64* pSize
            );

        __override virtual __checkReturn HRESULT Seek(
            UINT64 position
            );

        __override virtual __checkReturn HRESULT Read(
            __out_bcount_part(bufferSize, *pBytesWritten) void* pBuffer,
            UINT64 bufferSize,
            __out UINT64* pBytesWritten
            );

    protected:
        CFileResourceStream(
            );

        virtual ~CFileResourceStream(
            );

        __checkReturn HRESULT Initialize(
            __in FILE* pFile
            );

        FILE* m_pFile;
        UINT64 m_FileSize;
};

