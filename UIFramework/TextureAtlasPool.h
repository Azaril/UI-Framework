#pragma once

#include "Factory.h"
#include "Texture.h"
#include "TextureAllocator.h"
#include "TextureAtlas.h"

struct ITextureAtlasPool : public ITextureAllocator
{
    DECLARE_ADDREF_RELEASE_INTERFACE();
};

template< typename AtlasType >
class CTextureAtlasPool : public CRefCountedObjectBase< ITextureAtlasPool >
{
    public:
        DECLARE_FACTORY3( CTextureAtlasPool, UINT32, UINT32, typename AtlasType::TextureAllocatorInterface* );
    
        __override __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out ITexture** ppTexture
            )
        {
            HRESULT hr = S_OK;
            AtlasType* pNewAtlas = NULL;
            ITexture* pNewTexture = NULL;

            //TODO: Implement a better algorithm here, this will get slow as more textures get allocated and fragmentation happens.
            for (typename vector< AtlasType* >::reverse_iterator it = m_Textures.rbegin(); it != m_Textures.rend(); ++it)
            {
                AtlasType* pAtlas = (*it);

                if(SUCCEEDED(pAtlas->AllocateTexture(Width, Height, &pNewTexture)))
                {
                    break;
                }
            }

            if (pNewTexture == NULL)
            {
                IFC(CreateNewAtlas(Width + (AtlasType::Padding * 2), Height + (AtlasType::Padding * 2), &pNewAtlas));
            
                IFC(pNewAtlas->AllocateTexture(Width, Height, &pNewTexture));
            }

            *ppTexture = pNewTexture;
            pNewTexture = NULL;

        Cleanup:
            ReleaseObject(pNewAtlas);
            ReleaseObject(pNewTexture);

            return hr;        
        }
    
        __checkReturn HRESULT GetOrCreateFirstTextureAtlas(
            __deref_out AtlasType** ppAtlas
            )
        {
            HRESULT hr = S_OK;
            
            if (m_Textures.empty())
            {
                IFC(CreateNewAtlas(m_MinimumWidth, m_MinimumHeight, ppAtlas));
            }
            else
            {
                SetObject(*ppAtlas, m_Textures[0]);
            }
            
        Cleanup:
            return hr;
        }
    
    protected:
        CTextureAtlasPool(
            )
            : m_MinimumWidth(0)
            , m_MinimumHeight(0)
            , m_MaximumWidth(0)
            , m_MaximumHeight(0)
            , m_pTextureAllocator(NULL)
        {
        }    

        virtual ~CTextureAtlasPool(
            )
        {
            for (typename vector< AtlasType* >::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it)
            {
                (*it)->Release();
            }            
        }
    
        __checkReturn HRESULT Initialize(
            UINT32 MaximumWidth,
            UINT32 MaximumHeight,
            __in IBatchUpdateTextureAllocator* pAllocator
            )
        {
            HRESULT hr = S_OK;

            m_MaximumWidth = MaximumWidth;
            m_MaximumHeight = MaximumHeight;

            m_MinimumWidth = std::min(NextPowerOfTwo(m_MaximumWidth), (UINT32)1024);
            m_MinimumHeight = std::min(NextPowerOfTwo(m_MaximumHeight), (UINT32)1024);

            IFCEXPECT(m_MinimumWidth <= m_MaximumWidth);
            IFCEXPECT(m_MinimumHeight <= m_MaximumHeight);

            m_pTextureAllocator = pAllocator;

        Cleanup:
            return hr;            
        }
    
        __checkReturn HRESULT CreateNewAtlas(
            UINT32 Width,
            UINT32 Height,
            __deref_out AtlasType** ppAtlas
            )
        {
            HRESULT hr = S_OK;
            AtlasType* pNewAtlas = NULL;
            AtlasType::TextureInterface* pNewAtlasStorageTexture = NULL;
            UINT32 inflatedWidth = std::max(NextPowerOfTwo(Width), m_MinimumWidth);
            UINT32 inflatedHeight = std::max(NextPowerOfTwo(Height), m_MinimumHeight);

            IFCEXPECT(inflatedWidth <= m_MaximumWidth);
            IFCEXPECT(inflatedHeight <= m_MaximumHeight);
            
            IFC(m_pTextureAllocator->AllocateTexture(inflatedWidth, inflatedHeight, &pNewAtlasStorageTexture));
            
            IFC(AtlasType::Create(pNewAtlasStorageTexture, &pNewAtlas));
            
            SetObject(*ppAtlas, pNewAtlas);
            
            m_Textures.push_back(pNewAtlas);
            pNewAtlas = NULL;
            
        Cleanup:
            ReleaseObject(pNewAtlasStorageTexture);
            ReleaseObject(pNewAtlas);
            
            return hr;              
        }

        UINT32 NextPowerOfTwo(
            UINT32 val
            )
        {
            val--;

            val |= val >> 1;
            val |= val >> 2;
            val |= val >> 4;
            val |= val >> 8;
            val |= val >> 16;

            val++;

            return val;
        }
    
        typename AtlasType::TextureAllocatorInterface* m_pTextureAllocator;
        UINT32 m_MinimumWidth;
        UINT32 m_MinimumHeight;
        UINT32 m_MaximumWidth;
        UINT32 m_MaximumHeight;
        vector< AtlasType* > m_Textures;
};