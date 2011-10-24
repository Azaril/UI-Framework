#include "D3D9Texture.h"

PixelFormat::Value 
D3DFormatToPixelFormat(
    D3DFORMAT format
    )
{
    switch(format)
    {
        case D3DFMT_A8R8G8B8:
            {
                return PixelFormat::B8G8R8A8;
            }

        case D3DFMT_A8B8G8R8:
            {
                return PixelFormat::R8G8B8A8;
            }

        default:
            {
                return PixelFormat::Unknown;
            }
    }
}

CD3D9Texture::CD3D9Texture(
    )
    : m_pTexture(NULL)
    , m_Width(0)
    , m_Height(0)
    , m_Format(PixelFormat::Unknown)
{
}

CD3D9Texture::~CD3D9Texture(
    )
{
    ReleaseObject(m_pTexture);
}

__checkReturn HRESULT
CD3D9Texture::Initialize(
    __in IDirect3DTexture9* pTexture
    )
{
    HRESULT hr = S_OK;
    D3DSURFACE_DESC surfaceDescription = { };

    SetObject(m_pTexture, pTexture);

    IFC(m_pTexture->GetLevelDesc(0, &surfaceDescription));

    m_Width = surfaceDescription.Width;
    m_Height = surfaceDescription.Height;
    m_Format = D3DFormatToPixelFormat(surfaceDescription.Format);

Cleanup:
    return hr;
}

__out IDirect3DTexture9*
CD3D9Texture::GetD3DTexture( 
    )
{
    return m_pTexture;
}

__override UINT32 
CD3D9Texture::GetWidth(
    )
{
    return m_Width;
}

__override UINT32 
CD3D9Texture::GetHeight(
    )
{
    return m_Height;
}

__override PixelFormat::Value
CD3D9Texture::GetPixelFormat(
    )
{
    return m_Format;
}

__override __checkReturn HRESULT 
CD3D9Texture::SetData(
    __in_ecount(DataSize) BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;
    D3DLOCKED_RECT lockedRect = { };

    IFC(m_pTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD));

    {
        BYTE* pSourceData = pData;
        BYTE* pDestinationData = (BYTE*)lockedRect.pBits;
        UINT32 lineSize = PixelFormat::GetLineSize(m_Format, m_Width);

        for (UINT32 i = 0; i < m_Height; ++i)
        {
            memcpy(pDestinationData, pSourceData, lineSize);

            pSourceData += Stride;
            pDestinationData += lockedRect.Pitch;
        }
    }

    IFC(m_pTexture->UnlockRect(0));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9Texture::SetSubData(
    const RectU& Region,
    __in_ecount(DataSize) BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;
    D3DLOCKED_RECT lockedRect = { };
    RECT targetRect = { Region.left, Region.top, Region.right, Region.bottom };

    IFC(m_pTexture->LockRect(0, &lockedRect, &targetRect, 0));

    {
        BYTE* pSourceData = pData;
        BYTE* pDestinationData = (BYTE*)lockedRect.pBits;
        UINT32 regionLineSize = PixelFormat::GetLineSize(m_Format, Region.GetWidth());

        for (UINT32 i = 0; i < Region.GetHeight(); ++i)
        {
            memcpy(pDestinationData, pSourceData, regionLineSize);

            pSourceData += Stride;
            pDestinationData += lockedRect.Pitch;
        }
    }

    IFC(m_pTexture->UnlockRect(0));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD3D9Texture::SetMultipleSubData(
    __in_ecount(RegionCount) const RectU* pRegions,
    __in_ecount(RegionCount) BYTE** ppData,
    __in_ecount(RegionCount) UINT32* pDataSizes,
    __in_ecount(RegionCount) INT32* pStrides,
    UINT32 RegionCount
    )
{
    HRESULT hr = S_OK;

    //TODO: Is this optimal, is one lock better?
    for (UINT32 i = 0; i < RegionCount; ++i)
    {
        IFC(SetSubData(pRegions[i], ppData[i], pDataSizes[i], pStrides[i]));
    }

Cleanup:
    return hr;
}