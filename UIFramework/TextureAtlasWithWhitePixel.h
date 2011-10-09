#pragma once

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
        DECLARE_FACTORY1( CTextureAtlasWithWhitePixel, ITexture* );
    
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
            __in ITexture* pTexture
            )
        {
            HRESULT hr = S_OK;

	        IFC(BaseAtlasType::Initialize(pTexture));

	        IFC(AllocateTexture(1, 1, &m_pWhitePixel));

	        switch (m_pWhitePixel->GetPixelFormat())
	        {
		        case PixelFormat::B8G8R8A8:
			        {
				        BYTE whitePixel[] = { 0xFF, 0xFF, 0xFF, 0xFF };

				        IFC(m_pWhitePixel->SetData(whitePixel, sizeof(whitePixel), PixelFormat::GetLineSize(PixelFormat::B8G8R8A8, 1)));

				        break;
			        }

		        default:
			        {
				        IFC(E_UNEXPECTED);
			        }
	        }

        Cleanup:
	        return hr;
        }
    
        ITexture* m_pWhitePixel;
};