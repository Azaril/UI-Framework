#pragma once

#include "Texture.h"
#include "TextureAllocator.h"
#include "Factory.h"
#include "TextureAtlasView.h"
#include "TextureAtlasNode.h"

struct ITextureAtlas : public ITextureAllocator
{
    DECLARE_ADDREF_RELEASE_INTERFACE();

    __out virtual ITexture* GetTexture(
        ) = 0;
};

template< typename Base, UINT32 texturePadding >
class CTextureAtlas : public CRefCountedObjectBase< Base >
{
    public:
        DECLARE_FACTORY1( CTextureAtlas, ITexture* );

        static const UINT32 Padding = texturePadding;

        __override __checkReturn HRESULT AllocateTexture(
            UINT32 Width,
            UINT32 Height,
            __deref_out ITexture** ppTexture        
        	)
        {
            HRESULT hr = S_OK;
            CTextureAtlasView* pAtlasView = NULL;
            
            //
            // Inflate the requested size by the required padding, it will be deflated
            // before being passed to the allocated view.
            //
            const SizeU inflatedSize(Width + (texturePadding * 2), Height + (texturePadding * 2));

            IFC_NOTRACE(m_pRootNode->Allocate(this, inflatedSize, &pAtlasView));

            *ppTexture = pAtlasView;
            pAtlasView = NULL;

        Cleanup:
            ReleaseObject(pAtlasView);

            return hr;            
        }
    
        __override __out virtual ITexture* GetTexture(
            )
        {
            return m_pTexture;
        }
    
    protected:
        CTextureAtlas(
            )
            : m_pTexture(NULL)
            , m_pRootNode(NULL)
        {
        }

        virtual ~CTextureAtlas(
        	)
        {
            delete m_pRootNode;

            ReleaseObject(m_pTexture);            
        }

        __checkReturn HRESULT Initialize(
        	__in ITexture* pTexture
        	)
        {
            HRESULT hr = S_OK;

            m_pTexture = pTexture;
            AddRefObject(m_pTexture);

            m_pRootNode = new CTextureAtlasNode< texturePadding >(MakeRect(SizeU(pTexture->GetWidth(), pTexture->GetHeight())));
            IFCOOM(m_pRootNode);

        Cleanup:
            return hr;            
        }

        ITexture* m_pTexture;
        CTextureAtlasNode< texturePadding >* m_pRootNode;
};