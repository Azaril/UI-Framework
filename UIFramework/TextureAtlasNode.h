#pragma once

#include "Texture.h"
#include "Factory.h"

struct ITextureAtlas;

struct ITextureAtlasNode
{    
    virtual void NotifyViewDeleted(
        ) = 0;
};

template< UINT32 Padding >
class CTextureAtlasNode : private ITextureAtlasNode
{
    public:
        CTextureAtlasNode(
            const RectU& Rect
            )
            : m_Rect(Rect)
            , m_pLeftChild(NULL)
            , m_pRightChild(NULL)
            , m_pView(NULL)
        {
        }

        virtual ~CTextureAtlasNode(
            )
        {
            delete m_pLeftChild;
            delete m_pRightChild;            
        }

        const RectU& GetRect(
            )
        {
            return m_Rect;
        }

        __checkReturn HRESULT Allocate(
            __in ITextureAtlas* pAtlas,
            const SizeU& Size,
            __deref_out CTextureAtlasView** ppView
            )
        {
            HRESULT hr = S_OK;
            CTextureAtlasView* pNewView = NULL;

            //
            // Check if the size requested is larger in either dimension than this node contains.
            //
            if(Size.width > m_Rect.GetWidth() || Size.height > m_Rect.GetHeight())
            {
                IFC_NOTRACE(E_FAIL);
            }

            //
            // Attempt to allocate a view from the left child.
            //
            if (m_pLeftChild != NULL && SUCCEEDED(m_pLeftChild->Allocate(pAtlas, Size, ppView)))
            {
                goto Cleanup;
            }
            //
            // Attempt to allocate a view from the right child.
            //
            else if(m_pRightChild != NULL && SUCCEEDED(m_pRightChild->Allocate(pAtlas, Size, ppView)))
            {
                goto Cleanup;
            }
            //
            // Attempt to allocate a view from this node.
            //
            else if(m_pLeftChild == NULL && m_pRightChild == NULL)
            {
                //
                // If this node already has a texture view, we can't allocate another.
                //
                IFCEXPECT_NOTRACE(m_pView == NULL);

                //
                // If the size requested fits this node perfectly, allocate a view.
                //
                if (Size == m_Rect.GetSize())
                {
                    //
                    // Deflate the view rectangle by the specified padding.
                    //
                    RectU viewRect(m_Rect.left + Padding, m_Rect.top + Padding, m_Rect.right - Padding, m_Rect.bottom - Padding);
                    
                    IFC(CTextureAtlasView::Create(pAtlas, this, viewRect, &pNewView));

                    //
                    // NOTE: This node does not hold on to the view as when it is released we want
                    //       to coalesce the free space and we rely on the deletion of the view
                    //       to notify this node.
                    //
                    m_pView = pNewView;

                    //
                    // Return view to caller and transfer ownership.
                    //
                    *ppView = pNewView;
                    pNewView = NULL;
                }
                else
                {
                    UINT32 widthDifference = m_Rect.GetWidth() - Size.width;
                    UINT32 heightDifference = m_Rect.GetHeight() - Size.height;

                    //
                    // Check if splitting on height or width to keep the greatest dimension open.
                    //
                    if (widthDifference > heightDifference)
                    {
                        RectU left(m_Rect.left, m_Rect.top, m_Rect.left + Size.width, m_Rect.bottom);
                        RectU right(m_Rect.left + Size.width, m_Rect.top, m_Rect.right, m_Rect.bottom);

                        m_pLeftChild = new CTextureAtlasNode(left);
                        IFCOOM(m_pLeftChild);

                        m_pRightChild = new CTextureAtlasNode(right);
                        IFCOOM(m_pRightChild);
                    }
                    else
                    {
                        RectU top(m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.top + Size.height);
                        RectU bottom(m_Rect.left, m_Rect.top + Size.height, m_Rect.right, m_Rect.bottom);

                        m_pLeftChild = new CTextureAtlasNode(top);
                        IFCOOM(m_pLeftChild);

                        m_pRightChild = new CTextureAtlasNode(bottom);
                        IFCOOM(m_pRightChild);
                    }

                    //
                    // Allocate and get view from newly allocated node.
                    //
                    IFC(m_pLeftChild->Allocate(pAtlas, Size, ppView));
                }
            }

        Cleanup:
            ReleaseObject(pNewView);

            return hr;
        }

    protected:
        __override virtual void NotifyViewDeleted(
            )
        {
            m_pView = NULL;

            //TODO: Coalesce free regions...        
        }

        RectU m_Rect;
        CTextureAtlasView* m_pView;
        CTextureAtlasNode< Padding >* m_pLeftChild;
        CTextureAtlasNode< Padding >* m_pRightChild;
};