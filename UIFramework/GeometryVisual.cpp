#include "GeometryVisual.h"

CGeometryVisual::CGeometryVisual() : m_Geometry(NULL),
                                     m_Brush(NULL),
                                     m_GraphicsBrush(NULL)
{
}

CGeometryVisual::~CGeometryVisual()
{
    ReleaseObject(m_Geometry);
    ReleaseObject(m_Brush);
    ReleaseObject(m_GraphicsBrush);
}


HRESULT CGeometryVisual::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CVisual::Initialize());

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::SetGeometry(CGeometry* pGeometry)
{
    HRESULT hr = S_OK;

    IFC(InternalSetGeometry(pGeometry));

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::SetBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFC(InternalSetBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::InternalSetGeometry(CGeometry* pGeometry)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_Geometry);

    m_Geometry = pGeometry;

    AddRefObject(m_Geometry);

    return hr;
}

HRESULT CGeometryVisual::InternalSetBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_Brush);

    m_Brush = pBrush;

    AddRefObject(m_Brush);

    return hr;
}

HRESULT CGeometryVisual::PreRender(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    if(m_Brush != NULL && m_GraphicsBrush == NULL)
    {
        if(FAILED(m_Brush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &m_GraphicsBrush)))
        {
            m_GraphicsBrush = NULL;
        }
    }

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::RenderTransformed(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    
    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    if(m_Geometry != NULL && m_GraphicsBrush != NULL)
    {
        IFC(pRenderTarget->FillGeometry(m_Geometry, m_GraphicsBrush));
    }

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    BOOL FillContainsPoint = FALSE;

    IFCPTR(ppHitTestResult);

    if(m_Geometry != NULL)
    {
        //TODO: Fix this!
        using namespace D2D1;

        IFC(m_Geometry->FillContainsPoint(LocalPoint * m_FinalLocalTransform, &FillContainsPoint));
    }

    if(FillContainsPoint)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

Cleanup:
    return hr;
}