#include "RectangleVisual.h"

CRectangleVisual::CRectangleVisual() : m_Brush(NULL)
{
    m_Position.x = 0;
    m_Position.y = 0;

    m_Size.width = 0;
    m_Size.height = 0;
}

CRectangleVisual::~CRectangleVisual()
{
    ReleaseObject(m_Brush);
}

HRESULT CRectangleVisual::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CVisual::Initialize());

Cleanup:
    return hr;
}

HRESULT CRectangleVisual::SetBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_Brush);

    m_Brush = pBrush;

    AddRefObject(m_Brush);

    return hr;
}

HRESULT CRectangleVisual::SetSize(SizeF Size)
{
    HRESULT hr = S_OK;

    m_Size = Size;

    return hr;
}

HRESULT CRectangleVisual::SetPosition(Point2F Position)
{
    HRESULT hr = S_OK;

    m_Position = Position;

    return hr;
}

HRESULT CRectangleVisual::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    if(m_Brush != NULL)
    {
        RectF Rect = { m_Position.x, m_Position.y, m_Position.x + m_Size.width, m_Position.y + m_Size.height };

        //TODO: Add stroke behavior.
        IFC(pRenderTarget->FillRectangle(Rect, m_Brush));
    }

    IFC(CVisual::RenderTransformed(Context));

Cleanup:
    return hr;
}