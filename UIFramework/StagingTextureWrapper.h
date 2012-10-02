#pragma once

#include "Texture.h"
#include "TextureAllocator.h"
#include "Factory.h"
#include "StagingTextureCallback.h"

class CStagingTextureWrapper : public CRefCountedObjectBase< IBatchUpdateTexture >
{
    public:
        DECLARE_FACTORY3( CStagingTextureWrapper, ITextureAllocator*, IStagingTextureCallback*, ITexture* );

        __out ITexture* GetTargetTexture(
            );

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

    protected:
        CStagingTextureWrapper(
            );

        virtual ~CStagingTextureWrapper(
            );

        __checkReturn HRESULT Initialize(
            __in ITextureAllocator* pAllocator,
            __in IStagingTextureCallback* pCallback,
            __in ITexture* pTargetTexture
            );

        ITextureAllocator* m_pAllocator;
        IStagingTextureCallback* m_pCallback;
        ITexture* m_pTargetTexture;
};

