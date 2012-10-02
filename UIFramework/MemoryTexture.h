#pragma once

#include "Texture.h"
#include "Factory.h"

class CMemoryTexture : public CRefCountedObjectBase< IBatchUpdateTexture >
{
    public:
        DECLARE_FACTORY3( CMemoryTexture, PixelFormat::Value, UINT32, UINT32 );

        __override virtual UINT32 GetWidth(
            );

        __override virtual UINT32 GetHeight(
            );

        __override virtual PixelFormat::Value GetPixelFormat(
            );    

        __override virtual __checkReturn HRESULT SetData(
            __in_ecount(DataSize) const BYTE* pData,
            UINT32 DataSize,
            INT32 Stride
            );

        __override virtual __checkReturn HRESULT SetSubData(
            const RectU& Region,
            __in_ecount(DataSize) const BYTE* pData,
            UINT32 DataSize,
            INT32 Stride
            );  

        __override virtual __checkReturn HRESULT SetMultipleSubData(
            __in_ecount(RegionCount) const RectU* pRegions,
            __in_ecount(RegionCount) const BYTE** ppData,
            __in_ecount(RegionCount) const UINT32* pDataSizes,
            __in_ecount(RegionCount) const INT32* pStrides,
            UINT32 RegionCount
            );

		__out const BYTE* GetData(
			);

    protected:
        CMemoryTexture(
            );

        virtual ~CMemoryTexture(
            );

        __checkReturn HRESULT Initialize(
            PixelFormat::Value format,
			UINT32 width,
			UINT32 height
            );

        PixelFormat::Value m_Format;
        UINT32 m_Width;
        UINT32 m_Height;
		BYTE* m_pData;
};