#pragma once

#include "Types.h"
#include "RefCounted.h"

struct IReadStream
{
    DECLARE_ADDREF_RELEASE_INTERFACE();

    virtual __checkReturn HRESULT GetSize(
        __out UINT64* pSize
        ) = 0;

    virtual __checkReturn HRESULT Seek(
        UINT64 position
        ) =  0;

    virtual __checkReturn HRESULT Read(
        __out_bcount_part(bufferSize, *pBytesWritten) void* pBuffer,
        UINT64 bufferSize,
        __out UINT64* pBytesWritten
        ) = 0;
};

