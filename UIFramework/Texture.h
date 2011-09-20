#pragma once

#include "Types.h"

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
};