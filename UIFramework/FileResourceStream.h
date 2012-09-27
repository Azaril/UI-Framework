#pragma once

#include "ReadStream.h"
#include "RefCounted.h"
#include "Factory.h"

class UIFRAMEWORK_API CFileResourceStream : public CRefCountedObjectBase< IReadStream >
{
    public:
        DECLARE_FACTORY1( CFileResourceStream, FILE* );

        static __checkReturn HRESULT CreateOnPath(
            __in_z const WCHAR* pPath,
            __deref_out CFileResourceStream** ppStream
            );

        __override virtual __checkReturn HRESULT GetSize(
            __out UINT64* pSize
            );

        __override virtual __checkReturn HRESULT Seek(
            SeekType::Value seekType,
            INT64 position,
            __out_opt UINT64* pNewPosition
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
        INT64 m_FileSize;
};

