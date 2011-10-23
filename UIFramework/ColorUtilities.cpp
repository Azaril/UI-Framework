#include "ColorUtilities.h"
#include "StackHeapBuffer.h"

namespace ColorUtilities
{

template< size_t Size >
__checkReturn HRESULT 
FillBufferWithColor(
    StackHeapBuffer< BYTE, Size >& buffer,
    const RectU& region,
    PixelFormat::Value format,
    const ColorF& color,
    __out_opt UINT32* pBufferSize,
    __out_opt UINT32* pStride
    )
{
    HRESULT hr = S_OK;
    UINT32 width = region.GetWidth();
    UINT32 height = region.GetHeight();
    UINT32 lineSize = PixelFormat::GetLineSize(format, width);
    UINT32 bufferSize = lineSize * height;

    IFC(buffer.EnsureBufferSize(bufferSize));

    switch (format)
    {
        case PixelFormat::B8G8R8A8:
            {
                UINT32 blue = BYTE(color.b * 255) << 0;
                UINT32 green = BYTE(color.g * 255) << 8;
                UINT32 red = BYTE(color.r * 255) << 16;
                UINT32 alpha = BYTE(color.a * 255) << 24;

                UINT32 bufferColor = blue | green | red | alpha;

                BYTE* pDestinationLine = buffer.GetBuffer();

                for (UINT32 i = 0; i < height; ++i)
                {
                    UINT32* pDestination = (UINT32*)pDestinationLine;

                    for (UINT32 j = 0; j < width; ++j)
                    {
                        *pDestination = bufferColor;

                        ++pDestination;
                    }

                    pDestinationLine += lineSize;
                }

                break;
            }

        case PixelFormat::R8G8B8A8:
            {
                UINT32 red = BYTE(color.r * 255) << 0;
                UINT32 green = BYTE(color.g * 255) << 8;
                UINT32 blue = BYTE(color.b * 255) << 16;
                UINT32 alpha = BYTE(color.a * 255) << 24;

                UINT32 bufferColor = red | green | blue | alpha;

                BYTE* pDestinationLine = buffer.GetBuffer();

                for (UINT32 i = 0; i < height; ++i)
                {
                    UINT32* pDestination = (UINT32*)pDestinationLine;

                    for (UINT32 j = 0; j < width; ++j)
                    {
                        *pDestination = bufferColor;

                        ++pDestination;
                    }

                    pDestinationLine += lineSize;
                }

                break;
            }

        default:
            {
                IFC(E_UNEXPECTED);
            }
    }

    if (pBufferSize != NULL)
    {
        *pBufferSize = bufferSize;
    }

    if (pStride != NULL)
    {
        *pStride = lineSize;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
FillTextureWithColor(
    __in ITexture* pTexture,
    ColorF& color
    )
{
    HRESULT hr = S_OK;
    StackHeapBuffer< BYTE, 1024 * 4 > colorBuffer;
    UINT32 width = pTexture->GetWidth();
    UINT32 height = pTexture->GetHeight();
    PixelFormat::Value format = pTexture->GetPixelFormat();
    UINT32 bufferSize = 0;
    UINT32 stride = 0;

    IFC(FillBufferWithColor(colorBuffer, MakeRect(SizeU(width, height)), format, color, &bufferSize, &stride));

    IFC(pTexture->SetData(colorBuffer.GetBuffer(), bufferSize, stride));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
FillTextureWithColor(
    __in IBatchUpdateTexture* pTexture,
    __in_ecount(RegionCount) const RectU* pRegions,
    UINT32 RegionCount,
    ColorF& color
    )
{
    HRESULT hr = S_OK;
    StackHeapBuffer< UINT32, 16 > bufferSizes;
    StackHeapBuffer< INT32, 16 > strides;
    StackHeapBuffer< BYTE*, 16 > buffers;
    StackHeapBuffer< BYTE, 1024 * 4 > colorBuffer;
    UINT32 maxBufferSize = 0;
    PixelFormat::Value format = pTexture->GetPixelFormat();
    UINT32 pixelLineSize = PixelFormat::GetLineSize(format, 1);

    IFC(bufferSizes.EnsureBufferSize(RegionCount));
    IFC(strides.EnsureBufferSize(RegionCount));

    for (UINT32 i = 0; i < RegionCount; ++i)
    {
        UINT32 lineSize = PixelFormat::GetLineSize(format, pRegions[i].GetWidth());
        UINT32 bufferSize = lineSize * pRegions[i].GetHeight();

        strides.GetBuffer()[i] = lineSize;
        bufferSizes.GetBuffer()[i] = bufferSize;

        maxBufferSize = std::max(maxBufferSize, bufferSize);

        //
        // Ensure that the stride terminates on a pixel boundary as only a single buffer will be used
        // for all updates.
        //
        IFCEXPECT(lineSize % pixelLineSize == 0);
    }

    //
    // Fill buffer with color to be used for regions.
    //
    {
        RectU bufferRegion(MakeRect(SizeU(maxBufferSize / pixelLineSize, 1)));

        IFC(FillBufferWithColor(colorBuffer, bufferRegion, format, color, NULL, NULL));
    }

    for (UINT32 i = 0; i < RegionCount; ++i)
    {
        buffers.GetBuffer()[i] = colorBuffer.GetBuffer();
    }

    IFC(pTexture->SetMultipleSubData(pRegions, buffers.GetBuffer(), bufferSizes.GetBuffer(), strides.GetBuffer(), RegionCount)); 

Cleanup:
    return hr;
}

}