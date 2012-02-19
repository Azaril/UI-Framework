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
    , m_ForegroundBrushTransform(Matrix3X2F::Identity())
    , m_UpdateBrushTransform(TRUE)
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
    __in CProviders* pProviders,
    __in_opt CTextLayout* pTextLayout
    )
{
    HRESULT hr = S_OK;

    IFC(CVisual::Initialize(pProviders));

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

    m_UpdateBrushTransform = TRUE;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTextVisual::SetTextLayout(
    __in_opt CTextLayout* pTextLayout
    )
{
    HRESULT hr = S_OK;

    ReplaceObject(m_TextLayout, pTextLayout);

    m_UpdateBrushTransform = TRUE;

    return hr;
}

__checkReturn HRESULT 
CTextVisual::PreRender(
    CPreRenderContext& Context
    )
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    CTextLayoutMetrics* pLayoutMetrics = NULL;

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

        if (m_ForegroundGraphicsBrush != NULL)
        {
            m_UpdateBrushTransform = TRUE;
        }
    }

    //TODO: Remove this and get change notifications from layout?
    m_UpdateBrushTransform = TRUE; 

    if(m_UpdateBrushTransform && m_ForegroundGraphicsBrush != NULL)
    {
        RectF textBounds;

        IFC(m_TextLayout->GetMetrics(&pLayoutMetrics));

        IFC(pLayoutMetrics->GetBounds(&textBounds));

        {
            RectF brushBounds;

            IFC(m_ForegroundGraphicsBrush->GetBounds(brushBounds));

            {
                FLOAT TextWidth = textBounds.GetWidth();
                FLOAT TextHeight = textBounds.GetHeight();

                //TODO: Allow brush transforms?
                m_ForegroundBrushTransform = Matrix3X2F::Scale(TextWidth / brushBounds.GetWidth(), TextHeight / brushBounds.GetHeight()) * Matrix3X2F::Translation(textBounds.left, textBounds.top);
            }
        }

        m_UpdateBrushTransform = FALSE;
    }

    IFC(CVisual::PreRender(Context));

Cleanup:
    ReleaseObject(pLayoutMetrics);

    return hr;
}

HRESULT CTextVisual::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    IFC(CVisual::RenderTransformed(Context));

    if(m_TextLayout != NULL && m_ForegroundGraphicsBrush != NULL)
    {       
        IFC(m_ForegroundGraphicsBrush->SetTransform(m_ForegroundBrushTransform));

        IFC(pRenderTarget->RenderTextLayout(m_TextLayout, m_ForegroundGraphicsBrush));  
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