#pragma once

#include "TextureAtlas.h"
#include "TextureAtlas.h"

struct ITextureAtlasWithWhitePixel : public ITextureAtlas
{
    __out virtual ITexture* GetWhitePixelTexture(
        ) = 0;
};

template< UINT32 Padding >
class CTextureAtlasWithWhitePixel : public CTextureAtlas< ITextureAtlasWithWhitePixel, Padding >
{
    typedef CTextureAtlas< ITextureAtlasWithWhitePixel, Padding > BaseAtlasType;

    public:
        DECLARE_FACTORY1( CTextureAtlasWithWhitePixel, IBatchUpdateTexture* );
   
        __override __out ITexture* GetWhitePixelTexture(
            )
        {
            return m_pWhitePixel;
        }
    
    protected:
        CTextureAtlasWithWhitePixel(
            )
            : m_pWhitePixel(NULL)
        {
        }
    
        virtual ~CTextureAtlasWithWhitePixel(
            )
        {
            ReleaseObject(m_pWhitePixel);
        }
    
        __checkReturn HRESULT Initialize(
            __in IBatchUpdateTexture* pTexture
            )
        {
            HRESULT hr = S_OK;

	        IFC(BaseAtlasType::Initialize(pTexture));

	        IFC(BaseAtlasType::AllocateTexture(1, 1, &m_pWhitePixel));

            {
                ColorF whitePixelColor(1.0f, 1.0f, 1.0f, 1.0f);

                IFC(ColorUtilities::FillTextureWithColor(m_pWhitePixel, whitePixelColor));
            }

        Cleanup:
	        return hr;
        }
    
        ITexture* m_pWhitePixel;
};