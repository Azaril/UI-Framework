#pragma once

#include <d3d9.h>

#include "Texture.h"
#include "Factory.h"

class CD3D9Texture : public CRefCountedObjectBase< ITexture >
{
    public:
        DECLARE_FACTORY1( CD3D9Texture, IDirect3DTexture9* );

        __out IDirect3DTexture9* GetD3DTexture(
            );

        __override virtual UINT32 GetWidth(
            );

        __override virtual UINT32 GetHeight(
            );

        __override virtual PixelFormat::Value GetPixelFormat(
            );    

        __override virtual __checkReturn HRESULT SetData(
            __in_ecount(DataSize) BYTE* pData,
            UINT32 DataSize,
            INT32 Stride
            );

        __override virtual __checkReturn HRESULT SetSubData(
            const RectU& Region,
            __in_ecount(DataSize) BYTE* pData,
            UINT32 DataSize,
            INT32 Stride
            );  

    protected:
        CD3D9Texture(
            );

        virtual ~CD3D9Texture(
            );

        __checkReturn HRESULT Initialize(
            __in IDirect3DTexture9* pTexture
            );

        IDirect3DTexture9* m_pTexture;
        UINT32 m_Width;
        UINT32 m_Height;
        PixelFormat::Value m_Format;
};

