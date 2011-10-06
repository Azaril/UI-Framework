#pragma once

#include "Texture.h"
#include "TextureAllocator.h"
#include "Factory.h"

struct ITextureAtlas;
struct ITextureAtlasNode;

class CTextureAtlasView : public CRefCountedObjectBase< ITexture >
{
    public:
        DECLARE_FACTORY3( CTextureAtlasView, ITextureAtlas*, ITextureAtlasNode*, const RectU& );
    
        __out ITextureAtlas* GetAtlas(
            );
    
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
            __in ITextureAtlas* pAtlas,
            __in ITextureAtlasNode* pNode,
            const RectU& Rect
            );

        ITextureAtlas* m_pAtlas;
        ITextureAtlasNode* m_pNode;
        ITexture* m_pTexture;
        RectU m_Rect;
};