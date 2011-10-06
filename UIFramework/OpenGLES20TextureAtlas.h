#pragma once

#include "TextureAtlas.h"

class COpenGLES20TextureAtlas : public CTextureAtlas< 1 >
{
    public:
        DECLARE_FACTORY1( COpenGLES20TextureAtlas, ITexture* );
    
        __out ITexture* GetWhitePixelTexture(
            );
    
    protected:
        COpenGLES20TextureAtlas(
            );
    
        virtual ~COpenGLES20TextureAtlas(
            );
    
        __checkReturn HRESULT Initialize(
            __in ITexture* pTexture
            );
    
        ITexture* m_pWhitePixel;
};