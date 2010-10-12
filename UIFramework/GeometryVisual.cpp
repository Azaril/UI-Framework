#include "GeometryVisual.h"

CGeometryVisual::CGeometryVisual() : m_Geometry(NULL),
                                     m_FillBrush(NULL),
                                     m_StrokeBrush(NULL),
                                     m_FillGraphicsBrush(NULL),
                                     m_StrokeGraphicsBrush(NULL),
                                     m_StrokeThickness(1.0f),
                                     m_UpdateFillTransform(TRUE)
{
    m_FillBrushTransform = Matrix3X2F::Identity();
    m_ModifyFillBrushTransform = Matrix3X2F::Identity();
}

CGeometryVisual::~CGeometryVisual()
{
    ReleaseObject(m_Geometry);
    ReleaseObject(m_FillBrush);
    ReleaseObject(m_StrokeBrush);
    ReleaseObject(m_FillGraphicsBrush);
    ReleaseObject(m_StrokeGraphicsBrush);
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

HRESULT CGeometryVisual::SetFillBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFC(InternalSetFillBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::SetStrokeBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFC(InternalSetStrokeBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::SetStrokeThickness(FLOAT Thickness)
{
    HRESULT hr = S_OK;

    IFC(InternalSetStrokeThickness(Thickness));

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::SetFillBrushTransform(const Matrix3X2F& Transform)
{
    HRESULT hr = S_OK;

    m_ModifyFillBrushTransform = Transform;

    m_UpdateFillTransform = TRUE;

    return hr;
}

HRESULT CGeometryVisual::InternalSetGeometry(CGeometry* pGeometry)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_Geometry);

    m_Geometry = pGeometry;

    AddRefObject(m_Geometry);

    m_UpdateFillTransform = TRUE;

    return hr;
}

HRESULT CGeometryVisual::InternalSetFillBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    if(m_FillBrush)
    {
        IFC(RemoveVisualResource(m_FillBrush));

        ReleaseObject(m_FillBrush);
    }

    ReleaseObject(m_FillGraphicsBrush);

    m_FillBrush = pBrush;

    if(m_FillBrush)
    {
        AddRefObject(m_FillBrush);

        IFC(AddVisualResource(m_FillBrush));        
    }

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::InternalSetStrokeBrush(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    if(m_StrokeBrush)
    {
        IFC(RemoveVisualResource(m_StrokeBrush));

        ReleaseObject(m_StrokeBrush);
    }

    ReleaseObject(m_StrokeGraphicsBrush);

    m_StrokeBrush = pBrush;

    if(m_StrokeBrush)
    {
        IFC(AddVisualResource(m_StrokeBrush));

        AddRefObject(m_StrokeBrush);
    }

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::InternalSetStrokeThickness(FLOAT Thickness)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Thickness >= 0 );

    m_StrokeThickness = Thickness;

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::OnVisualNotification(CVisualNotification* pNotification)
{
    HRESULT hr = S_OK;

    IFCPTR(pNotification);

    switch(pNotification->GetType())
    {
        case VisualNotification::ChildBrushInvalidated:
            {
                CBrushInvalidatedNotification* pBrushInvalidatedNotification = (CBrushInvalidatedNotification*)pNotification;

                IFC(OnBrushInvalidated(pBrushInvalidatedNotification->GetBrush()));

                break;
            }
    }

    IFC(CVisual::OnVisualNotification(pNotification));

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::OnBrushInvalidated(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(pBrush);

    if(pBrush == m_FillBrush)
    {
        ReleaseObject(m_FillGraphicsBrush);
    }
    else if(pBrush == m_StrokeBrush)
    {
        ReleaseObject(m_StrokeGraphicsBrush);
    }

Cleanup:
    return hr;
}

HRESULT CGeometryVisual::PreRender(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;
    BOOL GenerateStrokeTransform = FALSE;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    if(m_FillBrush != NULL && m_FillGraphicsBrush == NULL)
    {
        if(FAILED(m_FillBrush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &m_FillGraphicsBrush)))
        {
            m_FillGraphicsBrush = NULL;
        }
        else
        {
            m_UpdateFillTransform = TRUE;
        }
    }

    if(m_StrokeBrush != NULL && m_StrokeGraphicsBrush == NULL)
    {
        if(FAILED(m_StrokeBrush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &m_StrokeGraphicsBrush)))
        {
            m_StrokeGraphicsBrush = NULL;
        }
        else
        {
            GenerateStrokeTransform = TRUE;
        }
    }

    if(m_UpdateFillTransform || GenerateStrokeTransform)
    {
        RectF GeometryBounds;

        IFC(m_Geometry->GetBounds(&GeometryBounds));

        if(m_UpdateFillTransform)
        {
            m_UpdateFillTransform = FALSE;

            m_FillBrushTransform = m_ModifyFillBrushTransform * Matrix3X2F::Scale(GeometryBounds.right - GeometryBounds.left, GeometryBounds.bottom - GeometryBounds.top) * Matrix3X2F::Translation(GeometryBounds.left, GeometryBounds.top);

            //FLOAT GeometryWidth = GeometryBounds.right - GeometryBounds.left;
            //FLOAT ScaleX = GeometryWidth / Bounds.width;

            //FLOAT GeometryHeight = GeometryBounds.bottom - GeometryBounds.top;
            //FLOAT ScaleY = GeometryHeight / Bounds.height;

            //m_FillBrushTransform = Matrix3X2F::Scale(ScaleX, ScaleY);
        }

        if(GenerateStrokeTransform)
        {
            m_StrokeBrushTransform = Matrix3X2F::Translation(GeometryBounds.left, GeometryBounds.top) * Matrix3X2F::Scale(GeometryBounds.right - GeometryBounds.left, GeometryBounds.bottom - GeometryBounds.top);
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

    if(m_Geometry != NULL)
    {
        if(m_FillGraphicsBrush != NULL)
        {
            IFC(m_FillGraphicsBrush->SetTransform(m_FillBrushTransform));

            IFC(pRenderTarget->FillGeometry(m_Geometry, m_FillGraphicsBrush));
        }

        if(m_StrokeGraphicsBrush != NULL)
        {
            IFC(m_StrokeGraphicsBrush->SetTransform(m_FillBrushTransform));

            IFC(pRenderTarget->DrawGeometry(m_Geometry, m_StrokeGraphicsBrush, m_StrokeThickness));
        }
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
        IFC(m_Geometry->FillContainsPoint(LocalPoint * m_FinalLocalTransform, &FillContainsPoint));
    }

    if(FillContainsPoint)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

Cleanup:
    return hr;
}