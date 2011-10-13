#pragma once

#include <d3d10.h>

#include "Texture.h"
#include "Factory.h"

class CD3D10Texture : public CRefCountedObjectBase< ITexture >
{
    public:
        DECLARE_FACTORY1( CD3D10Texture, ID3D10Texture2D* );

        __out ID3D10Texture2D* GetD3DTexture(
            );

        __out ID3D10ShaderResourceView* GetResourceView(
            );

        void SetResourceView(
            __in_opt ID3D10ShaderResourceView* pView
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
        CD3D10Texture(
            );

        virtual ~CD3D10Texture(
            );

        __checkReturn HRESULT Initialize(
            __in ID3D10Texture2D* pTexture
            );

        ID3D10Texture2D* m_pTexture;
        ID3D10ShaderResourceView* m_pResourceView;
        UINT32 m_Width;
        UINT32 m_Height;
        PixelFormat::Value m_Format;
};

