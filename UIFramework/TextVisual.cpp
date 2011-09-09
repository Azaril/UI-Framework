#include "TextVisual.h"

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CTextVisual, OnForegroundBrushChanged );

CTextVisual::CTextVisual(
    ) 
    : m_ForegroundBrush(NULL)
    , m_ForegroundGraphicsBrush(NULL)
    , m_TextLayout(NULL)
{
}

CTextVisual::~CTextVisual(
    )
{
    ReleaseObject(m_ForegroundBrush);
    ReleaseObject(m_ForegroundGraphicsBrush);
    ReleaseObject(m_TextLayout);
}

__checkReturn HRESULT 
CTextVisual::Initialize(
    __in_opt CTextLayout* pTextLayout
    )
{
    HRESULT hr = S_OK;

    IFC(CVisual::Initialize());

    IFC(SetTextLayout(pTextLayout));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextVisual::SetForegroundBrush(
    __in_opt CBrush* pBrush
    )
{
    HRESULT hr = S_OK;

    IFC(SetForegroundBrushInternal(pBrush));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextVisual::SetForegroundBrushInternal(
    __in_opt CBrush* pBrush
    )
{
    HRESULT hr = S_OK;

    if(m_ForegroundBrush)
    {
        IFC(RemoveVisualResource(m_ForegroundBrush, &INSTANCE_CHANGE_CALLBACK( CTextVisual, OnForegroundBrushChanged )));

        ReleaseObject(m_ForegroundBrush);
    }

    ReleaseObject(m_ForegroundGraphicsBrush);

    m_ForegroundBrush = pBrush;

    if(m_ForegroundBrush)
    {
        AddRefObject(m_ForegroundBrush);

        IFC(AddVisualResource(m_ForegroundBrush, &INSTANCE_CHANGE_CALLBACK( CTextVisual, OnForegroundBrushChanged )));        
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextVisual::SetTextLayout(
    __in_opt CTextLayout* pTextLayout
    )
{
    HRESULT hr = S_OK;

    ReleaseObject(m_TextLayout);

    m_TextLayout = pTextLayout;

    AddRefObject(m_TextLayout);

    return hr;
}

__checkReturn HRESULT 
CTextVisual::PreRender(
    CPreRenderContext& Context
    )
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    if(m_TextLayout != NULL && m_ForegroundGraphicsBrush == NULL)
    {
        if(m_ForegroundBrush)
        {
            if(FAILED(m_ForegroundBrush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &m_ForegroundGraphicsBrush)))
            {
                m_ForegroundGraphicsBrush = NULL;
            }
        }

        if(m_ForegroundGraphicsBrush == NULL)
        {
            IFC(pRenderTarget->GetDefaultBrush(DefaultBrush::TextForeground, &m_ForegroundGraphicsBrush));
        }
    }

    IFC(CVisual::PreRender(Context));

Cleanup:
    return hr;
}

HRESULT CTextVisual::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    Point2F TextOrigin;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    IFC(CVisual::RenderTransformed(Context));

    if(m_TextLayout != NULL && m_ForegroundGraphicsBrush != NULL)
    {
        IFC(pRenderTarget->RenderTextLayout(TextOrigin, m_TextLayout, m_ForegroundGraphicsBrush));  
    }

Cleanup:
    return hr;
}

HRESULT CTextVisual::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    SizeF RenderSize;

    IFCPTR(ppHitTestResult);

    //TODO: Implement hit testing.

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextVisual::OnForegroundBrushChanged(
    __in_opt CObjectWithType* pOldValue,
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    ReleaseObject(m_ForegroundGraphicsBrush);

    IFC(InvalidateVisual());

Cleanup:
    return hr;
}