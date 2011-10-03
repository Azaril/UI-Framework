#pragma once

#include "Texture.h"
#include "TextureAllocator.h"
#include "Factory.h"

struct ITextureAtlasNode;

class CTextureAtlasView : public CRefCountedObjectBase< ITexture >
{
    public:
        DECLARE_FACTORY3( CTextureAtlasView, ITextureAtlasNode*, ITexture*, const RectU& );
    
        __out ITexture* GetTexture(
            );
    
        const RectU& GetRect(
            );

        __override virtual UINT32 GetWidth(
            );

        __override virtual UINT32 GetHeight(
            );

        __override virtual INT32 GetStride(
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
        CTextureAtlasView(
            );

        virtual ~CTextureAtlasView(
            );

        __checkReturn HRESULT Initialize(
            __in  ITextureAtlasNode* pNode,
            __in ITexture* pTexture,
            const RectU& Rect
            );

        ITextureAtlasNode* m_pNode;
        ITexture* m_pTexture;
        RectU m_Rect;
};