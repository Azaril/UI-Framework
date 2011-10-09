#include "TextureAtlasView.h"
#include "TextureAtlas.h"

CTextureAtlasView::CTextureAtlasView( 
    )
    : m_pNode(NULL)
    , m_pAtlas(NULL)
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
    __in ITextureAtlas* pAtlas,
    __in ITextureAtlasNode* pNode,
    const RectU& Rect
    )
{
    HRESULT hr = S_OK;

    m_pAtlas = pAtlas;
    m_pNode = pNode;

    SetObject(m_pTexture, pAtlas->GetTexture());

    m_Rect = Rect;
    
    return hr;
}

__out ITexture*
CTextureAtlasView::GetTexture(
    )
{
    return m_pTexture;
}

__out ITextureAtlas*
CTextureAtlasView::GetAtlas(
    )
{
    return m_pAtlas;
}

const RectU&
CTextureAtlasView::GetRect(
    )
{
    return m_Rect;
}

__override UINT32 
CTextureAtlasView::GetWidth(
    )
{
    return m_Rect.GetWidth();
}

__override UINT32 
CTextureAtlasView::GetHeight(
    )
{
    return m_Rect.GetHeight();
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

    IFC(m_pTexture->SetSubData(m_Rect, pData, DataSize, Stride));

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

    Point2U modifiedPosition = m_Rect.GetTopLeft() + Region.GetTopLeft();
    RectU modifiedRect = MakeRect(modifiedPosition, Region.GetSize());

    IFC(m_pTexture->SetSubData(modifiedRect, pData, DataSize, Stride));

Cleanup:
    return hr;
}