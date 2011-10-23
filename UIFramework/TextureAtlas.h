#pragma once

#include "Texture.h"
#include "TextureAllocator.h"
#include "Factory.h"
#include "TextureAtlasView.h"
#include "TextureAtlasNode.h"
#include "ColorUtilities.h"

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
        DECLARE_FACTORY1( CTextureAtlas, IBatchUpdateTexture* );

        typedef IBatchUpdateTexture TextureInterface;
        typedef IBatchUpdateTextureAllocator TextureAllocatorInterface;

        static const UINT32 Padding = texturePadding;

        __override virtual __checkReturn HRESULT AllocateTexture(
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

#pragma warning(push)
#pragma warning(disable: 4127)
            if (texturePadding > 0)
#pragma warning(pop)
            {
                const RectU& viewRegion = pAtlasView->GetRect();

                const RectU regions[] =
                {
                    // Top
                    RectU(viewRegion.left - texturePadding, viewRegion.top - texturePadding, viewRegion.right + texturePadding, viewRegion.top),
                    // Bottom
                    RectU(viewRegion.left - texturePadding, viewRegion.bottom, viewRegion.right + texturePadding, viewRegion.bottom + texturePadding),
                    // Left
                    RectU(viewRegion.left - texturePadding, viewRegion.top, viewRegion.left, viewRegion.bottom),
                    // Right
                    RectU(viewRegion.right, viewRegion.top, viewRegion.right + texturePadding, viewRegion.bottom)
                };

                {
                    ColorF whiteGutterColor(1.0f, 1.0f, 1.0f, 1.0f);

                    IFC(ColorUtilities::FillTextureWithColor(m_pTexture, regions, ARRAYSIZE(regions), whiteGutterColor));
                }
            }

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
        	__in IBatchUpdateTexture* pTexture
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

        IBatchUpdateTexture* m_pTexture;
        CTextureAtlasNode< texturePadding >* m_pRootNode;
};