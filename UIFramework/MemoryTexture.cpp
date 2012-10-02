#include "MemoryTexture.h"

CMemoryTexture::CMemoryTexture(
    )
    : m_Format(PixelFormat::Unknown)
    , m_Width(0)
    , m_Height(0)
    , m_pData(NULL)
{
}

CMemoryTexture::~CMemoryTexture(
    )
{
    delete [] m_pData;
}

__checkReturn HRESULT
CMemoryTexture::Initialize(
	PixelFormat::Value format,
	UINT32 width,
	UINT32 height
    )
{
    HRESULT hr = S_OK;
	UINT32 lineSize = 0;
	UINT32 dataSize = 0;

    m_Format = format;
    m_Width = width;
    m_Height = height;

	lineSize = PixelFormat::GetLineSize(m_Format, m_Width);
	dataSize = lineSize * m_Height;

	m_pData = new BYTE[dataSize];
	IFCOOM(m_pData);

Cleanup:
    return hr;
}

__override UINT32 
CMemoryTexture::GetWidth(
    )
{
    return m_Width;
}

__override UINT32 
CMemoryTexture::GetHeight(
    )
{
    return m_Height;
}

__override PixelFormat::Value
CMemoryTexture::GetPixelFormat(
    )
{
    return m_Format;
}

__override __checkReturn HRESULT 
CMemoryTexture::SetData(
    __in_ecount(DataSize) const BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;

    {
        const BYTE* pSourceData = pData;
        BYTE* pDestinationData = (BYTE*)m_pData;

        UINT32 lineSize = PixelFormat::GetLineSize(m_Format, m_Width);

        for (UINT32 i = 0; i < m_Height; ++i)
        {
            memcpy(pDestinationData, pSourceData, lineSize);

            pSourceData += Stride;
            pDestinationData += lineSize;
        }
    }

    return hr;
}

__override __checkReturn HRESULT 
CMemoryTexture::SetSubData(
    const RectU& Region,
    __in_ecount(DataSize) const BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;

    //TODO: Validate data size.

    {
		UINT32 lineSize = PixelFormat::GetLineSize(m_Format, m_Width);

        const BYTE* pSourceData = pData;
        BYTE* pDestinationData = ((BYTE*)m_pData) + (lineSize * Region.top) + PixelFormat::GetLineSize(m_Format, Region.left);

        UINT32 regionLineSize = PixelFormat::GetLineSize(m_Format, Region.GetWidth());

        for (UINT32 i = 0; i < Region.GetHeight(); ++i)
        {
            memcpy(pDestinationData, pSourceData, regionLineSize);

            pSourceData += Stride;
            pDestinationData += lineSize;
        }
    }

    return hr;
}

__override __checkReturn HRESULT 
CMemoryTexture::SetMultipleSubData(
    __in_ecount(RegionCount) const RectU* pRegions,
    __in_ecount(RegionCount) const BYTE** ppData,
    __in_ecount(RegionCount) const UINT32* pDataSizes,
    __in_ecount(RegionCount) const INT32* pStrides,
    UINT32 RegionCount
    )
{
	HRESULT hr = S_OK;

	for (UINT32 i = 0; i < RegionCount; ++i)
	{
		IFC(SetSubData(pRegions[i], ppData[i], pDataSizes[i], pStrides[i]));
	}

Cleanup:
	return hr;
}

__out const BYTE*
CMemoryTexture::GetData(
	)
{
	return m_pData;
}