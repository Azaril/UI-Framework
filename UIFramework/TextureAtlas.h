#pragma once

#include "Texture.h"
#include "TextureAllocator.h"
#include "Factory.h"

class CTextureAtlasNode;

class CTextureAtlasView : public CRefCountedObjectBase< ITexture >
{
    public:
        DECLARE_FACTORY2( CTextureAtlasView, CTextureAtlasNode*, ITexture* );

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
            __in  CTextureAtlasNode* pNode,
            __in ITexture* pTexture
            );

        CTextureAtlasNode* m_pNode;
        ITexture* m_pTexture;
};

class CTextureAtlasNode
{
    friend class CTextureAtlasView;

    public:
        CTextureAtlasNode(
            const RectU& Rect
            );

        virtual ~CTextureAtlasNode(
            );

        const RectU& GetRect(
            );

        __checkReturn HRESULT Allocate(
            __in ITexture* pTexture,
            const SizeU& Size,
            __deref_out CTextureAtlasView** ppView
            );

    protected:
        void NotifyViewDeleted(
            );

        RectU m_Rect;
        CTextureAtlasView* m_pView;
        CTextureAtlasNode* m_pLeftChild;
        CTextureAtlasNode* m_pRightChild;
};

class CTextureAtlas : public CRefCountedObjectBase< ITextureAllocator >
{
    public:
        DECLARE_FACTORY1( CTextureAtlas, ITexture* );

        __override __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out ITexture** ppTexture        
        	);
    
    protected:
        CTextureAtlas(
            );

        virtual ~CTextureAtlas(
        	);

        __checkReturn HRESULT Initialize(
        	__in ITexture* pTexture
        	);

        ITexture* m_pTexture;
        CTextureAtlasNode* m_pRootNode;
};