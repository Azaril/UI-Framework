#pragma once

#include "Types.h"
#include "RefCounted.h"

namespace PixelFormat 
{
    enum Value
    {
        Unknown,
        B8G8R8A8
    };
}

struct ITexture
{
    DECLARE_ADDREF_RELEASE_INTERFACE();
    
    virtual UINT32 GetWidth(
        ) = 0;
    
    virtual UINT32 GetHeight(
        ) = 0;
    
    virtual INT32 GetStride(
        ) = 0;
    
    virtual PixelFormat::Value GetPixelFormat(
        ) = 0;
    
    virtual __checkReturn HRESULT SetData(
        __in_ecount(DataSize) BYTE* pData,
        UINT32 DataSize,
        INT32 Stride
        ) = 0;
    
    virtual __checkReturn HRESULT SetSubData(
        const RectU& Region,
        __in_ecount(DataSize) BYTE* pData,
        UINT32 DataSize,
        INT32 Stride
        ) = 0;
};