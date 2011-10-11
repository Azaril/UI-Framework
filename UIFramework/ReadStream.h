#pragma once

#include "Types.h"
#include "RefCounted.h"

namespace SeekType
{
    enum Value
    {
        Begin,
        Current,
        End
    };
}

struct IReadStream
{
    DECLARE_ADDREF_RELEASE_INTERFACE();

    virtual __checkReturn HRESULT GetSize(
        __out UINT64* pSize
        ) = 0;

    virtual __checkReturn HRESULT Seek(
        SeekType::Value seekType,
        INT64 position,
        __out_opt UINT64* pNewPosition
        ) =  0;

    virtual __checkReturn HRESULT Read(
        __out_bcount_part(bufferSize, *pBytesWritten) void* pBuffer,
        UINT64 bufferSize,
        __out UINT64* pBytesWritten
        ) = 0;
};

