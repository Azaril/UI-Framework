#include "D3D10Texture.h"

#if defined(FRAMEWORK_D3D10)

namespace
{

PixelFormat::Value 
DXGIFormatToPixelFormat(
    DXGI_FORMAT format
    )
{
    switch(format)
    {
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            {
                return PixelFormat::B8G8R8A8;
            }

        case DXGI_FORMAT_R8G8B8A8_UNORM:
            {
                return PixelFormat::R8G8B8A8;
            }

        default:
            {
                return PixelFormat::Unknown;
            }
    }
}

}

CD3D10Texture::CD3D10Texture(
    )
    : m_pTexture(NULL)
    , m_Width(0)
    , m_Height(0)
    , m_Format(PixelFormat::Unknown)
    , m_pResourceView(NULL)
{
}

CD3D10Texture::~CD3D10Texture(
    )
{
    ReleaseObject(m_pResourceView);
    ReleaseObject(m_pTexture);
}

__checkReturn HRESULT
CD3D10Texture::Initialize(
    __in ID3D10Texture2D* pTexture
    )
{
    HRESULT hr = S_OK;
    D3D10_TEXTURE2D_DESC textureDescription = { };

    SetObject(m_pTexture, pTexture);

    m_pTexture->GetDesc(&textureDescription);

    m_Width = textureDescription.Width;
    m_Height = textureDescription.Height;
    m_Format = DXGIFormatToPixelFormat(textureDescription.Format);

    return hr;
}

__out ID3D10Texture2D*
CD3D10Texture::GetD3DTexture( 
    )
{
    return m_pTexture;
}

__out ID3D10ShaderResourceView* 
CD3D10Texture::GetResourceView(
    )
{
    return m_pResourceView;
}

void 
CD3D10Texture::SetResourceView(
    __in_opt ID3D10ShaderResourceView* pView
    )
{
    ReplaceObject(m_pResourceView, pView);
}

__override UINT32 
CD3D10Texture::GetWidth(
    )
{
    return m_Width;
}

__override UINT32 
CD3D10Texture::GetHeight(
    )
{
    return m_Height;
}

__override PixelFormat::Value
CD3D10Texture::GetPixelFormat(
    )
{
    return m_Format;
}

__override __checkReturn HRESULT 
CD3D10Texture::SetData(
    __in_ecount(DataSize) const BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;
    D3D10_MAPPED_TEXTURE2D  mappedTexture = { };

    IFC(m_pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTexture));

    {
        const BYTE* pSourceData = pData;
        BYTE* pDestinationData = (BYTE*)mappedTexture.pData;
        UINT32 lineSize = PixelFormat::GetLineSize(m_Format, m_Width);

        for (UINT32 i = 0; i < m_Height; ++i)
        {
            memcpy(pDestinationData, pSourceData, lineSize);

            pSourceData += Stride;
            pDestinationData += mappedTexture.RowPitch;
        }
    }

Cleanup:
    if (mappedTexture.pData != NULL)
    {
        m_pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));
    }

    return hr;
}

__override __checkReturn HRESULT 
CD3D10Texture::SetSubData(
    const RectU& Region,
    __in_ecount(DataSize) const BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;
    D3D10_MAPPED_TEXTURE2D  mappedTexture = { };

    //TODO: Validate data size.

    IFC(m_pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE, 0, &mappedTexture));

    {
        const BYTE* pSourceData = pData;
        BYTE* pDestinationData = ((BYTE*)mappedTexture.pData) + (mappedTexture.RowPitch * Region.top) + PixelFormat::GetLineSize(m_Format, Region.left);
        UINT32 regionLineSize = PixelFormat::GetLineSize(m_Format, Region.GetWidth());

        for (UINT32 i = 0; i < Region.GetHeight(); ++i)
        {
            memcpy(pDestinationData, pSourceData, regionLineSize);

            pSourceData += Stride;
            pDestinationData += mappedTexture.RowPitch;
        }
    }

Cleanup:
    if (mappedTexture.pData != NULL)
    {
        m_pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));
    }

    return hr;
}

__override __checkReturn HRESULT 
CD3D10Texture::SetMultipleSubData(
    __in_ecount(RegionCount) const RectU* pRegions,
    __in_ecount(RegionCount) const BYTE** ppData,
    __in_ecount(RegionCount) const UINT32* pDataSizes,
    __in_ecount(RegionCount) const INT32* pStrides,
    UINT32 RegionCount
    )
{
    HRESULT hr = S_OK;
    D3D10_MAPPED_TEXTURE2D  mappedTexture = { };

    //TODO: Validate data size.

    IFC(m_pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE, 0, &mappedTexture));

    for (UINT32 i = 0; i < RegionCount; ++i)
    {
        const BYTE* pSourceData = ppData[i];
        BYTE* pDestinationData = ((BYTE*)mappedTexture.pData) + (mappedTexture.RowPitch * pRegions[i].top) + PixelFormat::GetLineSize(m_Format, pRegions[i].left);
        UINT32 regionLineSize = PixelFormat::GetLineSize(m_Format, pRegions[i].GetWidth());

        for (UINT32 i = 0; i < pRegions[i].GetHeight(); ++i)
        {
            memcpy(pDestinationData, pSourceData, regionLineSize);

            pSourceData += pStrides[i];
            pDestinationData += mappedTexture.RowPitch;
        }
    }

Cleanup:
    if (mappedTexture.pData != NULL)
    {
        m_pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));
    }

    return hr;
}

#endif