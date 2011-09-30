#pragma once

#include "Texture.h"
#include "TextureAllocator.h"
#include "Factory.h"

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
};