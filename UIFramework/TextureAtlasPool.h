#pragma once

#include "Factory.h"
#include "Texture.h"
#include "TextureAllocator.h"
#include "TextureAtlas.h"

class CTextureAtlasPool : public CRefCountedObjectBase< ITextureAllocator >
{
    public:
        DECLARE_FACTORY4( CTextureAtlasPool, UINT32, UINT32, UINT32, ITextureAllocator* );
    
        __override __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out ITexture** ppTexture
            );
    
    protected:
        CTextureAtlasPool(
            );
    
        virtual ~CTextureAtlasPool(
            );
    
        __checkReturn HRESULT Initialize(
            UINT32 Width,
            UINT32 Height,
            UINT32 Padding,
            __in ITextureAllocator* pAllocator
            );
    
        ITextureAllocator* m_pTextureAllocator;
        UINT32 m_Width;
        UINT32 m_Height;
        UINT32 m_Padding;
        std::vector< CTextureAtlas* > m_Textures;
};