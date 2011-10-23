#pragma once

#include "Types.h"
#include "RefCounted.h"

namespace PixelFormat 
{
    enum Value
    {
        Unknown,
        B8G8R8A8,
        R8G8B8A8
    };
        
    inline UINT32 GetLineSize(
        const PixelFormat::Value& format,
        UINT32 width
        )
    {
        switch (format)
        {
            case PixelFormat::B8G8R8A8:
            case PixelFormat::R8G8B8A8:
                return 4 * width;
                
            default:
                return 0;
        }
    }
}

struct ITexture
{
    DECLARE_ADDREF_RELEASE_INTERFACE();
    
    virtual UINT32 GetWidth(
        ) = 0;
    
    virtual UINT32 GetHeight(
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

struct IBatchUpdateTexture : public ITexture
{
    virtual __checkReturn HRESULT SetMultipleSubData(
        __in_ecount(RegionCount) const RectU* pRegions,
        __in_ecount(RegionCount) BYTE** ppData,
        __in_ecount(RegionCount) UINT32* pDataSizes,
        __in_ecount(RegionCount) INT32* pStrides,
        UINT32 RegionCount
        ) = 0;
};