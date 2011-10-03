#pragma once

#include "Factory.h"
#include "Texture.h"
#include "TextureAllocator.h"
#include "TextureAtlas.h"

template< UINT32 Padding >
class CTextureAtlasPool : public CRefCountedObjectBase< ITextureAllocator >
{
    typedef CTextureAtlas< Padding > AtlasType;
    
    public:
        DECLARE_FACTORY3( CTextureAtlasPool, UINT32, UINT32, ITextureAllocator* );
    
        __override __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out ITexture** ppTexture
            )
        {
            HRESULT hr = S_OK;
            ITexture* pNewAtlasStorageTexture = NULL;
            AtlasType* pNewAtlas = NULL;

            //TODO: Implement a better algorithm here, this will get slow as more textures get allocated and fragmentation happens.
            for (typename std::vector< AtlasType* >::reverse_iterator it = m_Textures.rbegin(); it != m_Textures.rend(); ++it)
            {
                AtlasType* pAtlas = (*it);

                if(SUCCEEDED(pAtlas->AllocateTexture(Width, Height, ppTexture)))
                {
                    goto Cleanup;
                }
            }

            IFC(m_pTextureAllocator->AllocateTexture(m_Width, m_Height, &pNewAtlasStorageTexture));

            IFC(AtlasType::Create(pNewAtlasStorageTexture, &pNewAtlas));

            IFC(pNewAtlas->AllocateTexture(Width, Height, ppTexture));

            m_Textures.push_back(pNewAtlas);
            pNewAtlas = NULL;

            Cleanup:
            ReleaseObject(pNewAtlasStorageTexture);

            return hr;        
        }
    
    protected:
        CTextureAtlasPool(
            )
            : m_Width(0)
            , m_Height(0)
            , m_pTextureAllocator(NULL)
        {
        }    

        virtual ~CTextureAtlasPool(
            )
        {
            for (typename std::vector< AtlasType* >::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it)
            {
                (*it)->Release();
            }            
        }
    
        __checkReturn HRESULT Initialize(
            UINT32 Width,
            UINT32 Height,
            __in ITextureAllocator* pAllocator
            )
        {
            HRESULT hr = S_OK;

            m_Width = Width;
            m_Height = Height;
            m_pTextureAllocator = pAllocator;

            return hr;            
        }
    
        ITextureAllocator* m_pTextureAllocator;
        UINT32 m_Width;
        UINT32 m_Height;
        std::vector< AtlasType* > m_Textures;
};