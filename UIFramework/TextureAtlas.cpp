#include "TextureAtlas.h"

CTextureAtlas::CTextureAtlas(
	)
	: m_pTexture(NULL)
    , m_pRootNode(NULL)
{
}

CTextureAtlas::~CTextureAtlas(
	)
{
    delete m_pRootNode;

	ReleaseObject(m_pTexture);
}

__checkReturn HRESULT
CTextureAtlas::Initialize(
	__in ITexture* pTexture
	)
{
	HRESULT hr = S_OK;

	m_pTexture = pTexture;
	AddRefObject(m_pTexture);

    m_pRootNode = new CTextureAtlasNode(MakeRect(SizeU(pTexture->GetWidth(), pTexture->GetHeight())));
    IFCOOM(m_pRootNode);

Cleanup:
	return hr;
}

__override __checkReturn HRESULT
CTextureAtlas::AllocateTexture(
	UINT32 Width,
	UINT32 Height,
	__deref_out ITexture** ppTexture
	)
{
	HRESULT hr = S_OK;
    CTextureAtlasView* pAtlasView = NULL;

    IFC(m_pRootNode->Allocate(m_pTexture, SizeU(Width, Height), &pAtlasView));

    *ppTexture = pAtlasView;
    pAtlasView = NULL;

Cleanup:
    ReleaseObject(pAtlasView);

	return hr;
}




CTextureAtlasNode::CTextureAtlasNode(
    const RectU& Rect
    )
    : m_Rect(Rect)
    , m_pLeftChild(NULL)
    , m_pRightChild(NULL)
    , m_pView(NULL)
{
}

CTextureAtlasNode::~CTextureAtlasNode(
    )
{
    delete m_pLeftChild;
    delete m_pRightChild;
}

const RectU& 
CTextureAtlasNode::GetRect(
    )
{
    return m_Rect;
}

__checkReturn HRESULT
CTextureAtlasNode::Allocate(
    __in ITexture* pTexture,
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
    if (m_pLeftChild != NULL && SUCCEEDED(m_pLeftChild->Allocate(pTexture, Size, ppView)))
    {
        goto Cleanup;
    }
    //
    // Attempt to allocate a view from the right child.
    //
    else if(m_pRightChild != NULL && SUCCEEDED(m_pRightChild->Allocate(pTexture, Size, ppView)))
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
        IFCEXPECT_NOTRACE(m_pView != NULL);

        //
        // If the size requested fits this node perfectly, allocate a view.
        //
        if (Size == m_Rect.GetSize())
        {
            IFC(CTextureAtlasView::Create(this, pTexture, &pNewView));

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
            IFC(m_pLeftChild->Allocate(pTexture, Size, ppView));
        }
    }

Cleanup:
    ReleaseObject(pNewView);

    return hr;
}

void
CTextureAtlasNode::NotifyViewDeleted(
    )
{
    m_pView = NULL;

    //TODO: Coalesce free regions...
}



CTextureAtlasView::CTextureAtlasView( 
    )
    : m_pNode(NULL)
{
}

CTextureAtlasView::~CTextureAtlasView(
    )
{
    ReleaseObject(m_pTexture);

    m_pNode->NotifyViewDeleted();
}

__checkReturn HRESULT
CTextureAtlasView::Initialize(
    __in CTextureAtlasNode* pNode,
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;

    m_pNode = pNode;

    SetObject(m_pTexture, pTexture);

    return hr;
}

__override UINT32 
CTextureAtlasView::GetWidth(
    )
{
    return m_pNode->GetRect().GetWidth();
}

__override UINT32 
CTextureAtlasView::GetHeight(
    )
{
    return m_pNode->GetRect().GetHeight();
}

__override INT32 
CTextureAtlasView::GetStride(
    )
{
    return m_pTexture->GetStride();
}

__override PixelFormat::Value 
CTextureAtlasView::GetPixelFormat(
    )
{
    return m_pTexture->GetPixelFormat();
}

__override __checkReturn HRESULT 
CTextureAtlasView::SetData(
    __in_ecount(DataSize) BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;

    IFC(m_pTexture->SetSubData(m_pNode->GetRect(), pData, DataSize, Stride));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CTextureAtlasView::SetSubData(
    const RectU& Region,
    __in_ecount(DataSize) BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;

    const RectU& subRect = m_pNode->GetRect();

    Point2U modifiedPosition = subRect.GetTopLeft() + Region.GetTopLeft();
    RectU modifiedRect = MakeRect(modifiedPosition, Region.GetSize());

    IFC(m_pTexture->SetSubData(modifiedRect, pData, DataSize, Stride));

Cleanup:
    return hr;
}