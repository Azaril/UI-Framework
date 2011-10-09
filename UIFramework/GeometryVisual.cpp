#include "GeometryVisual.h"

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CGeometryVisual, OnFillBrushChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CGeometryVisual, OnStrokeBrushChanged );

CGeometryVisual::CGeometryVisual(
    ) 
    : m_Geometry(NULL)
    , m_FillBrush(NULL)
    , m_StrokeBrush(NULL)
    , m_FillGraphicsBrush(NULL)
    , m_StrokeGraphicsBrush(NULL)
    , m_StrokeThickness(1.0f)
    , m_UpdateFillTransform(TRUE)
    , m_UpdateStrokeTransform(TRUE)
{
    m_FillBrushTransform = Matrix3X2F::Identity();
    m_StrokeBrushTransform = Matrix3X2F::Identity();
    m_BrushTransform = Matrix3X2F::Identity();
}

CGeometryVisual::~CGeometryVisual(
    )
{
    ReleaseObject(m_Geometry);
    ReleaseObject(m_FillBrush);
    ReleaseObject(m_StrokeBrush);
    ReleaseObject(m_FillGraphicsBrush);
    ReleaseObject(m_StrokeGraphicsBrush);
}


__checkReturn HRESULT 
CGeometryVisual::Initialize(
    )
{
    HRESULT hr = S_OK;

    IFC(CVisual::Initialize());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::SetGeometry(
    __in_opt CGraphicsGeometry* pGeometry
    )
{
    HRESULT hr = S_OK;

    IFC(InternalSetGeometry(pGeometry));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::SetFillBrush(
    __in_opt CBrush* pBrush
    )
{
    HRESULT hr = S_OK;

    IFC(InternalSetFillBrush(pBrush));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::SetStrokeBrush(
    __in_opt CBrush* pBrush
    )
{
    HRESULT hr = S_OK;

    IFC(InternalSetStrokeBrush(pBrush));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::SetStrokeThickness(
    FLOAT Thickness
    )
{
    HRESULT hr = S_OK;

    IFC(InternalSetStrokeThickness(Thickness));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CGeometryVisual::SetBrushTransform(
    const Matrix3X2F& Transform
    )
{
    HRESULT hr = S_OK;

    m_BrushTransform = Transform;

    m_UpdateFillTransform = TRUE;
    m_UpdateStrokeTransform = TRUE;

    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::InternalSetGeometry(
    __in CGraphicsGeometry* pGeometry
    )
{
    HRESULT hr = S_OK;

    ReleaseObject(m_Geometry);

    m_Geometry = pGeometry;

    AddRefObject(m_Geometry);

    m_UpdateFillTransform = TRUE;

    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::InternalSetFillBrush(
    __in CBrush* pBrush
    )
{
    HRESULT hr = S_OK;

    if(m_FillBrush)
    {
        IFC(RemoveVisualResource(m_FillBrush, &INSTANCE_CHANGE_CALLBACK( CGeometryVisual, OnFillBrushChanged )));

        ReleaseObject(m_FillBrush);
    }

    ReleaseObject(m_FillGraphicsBrush);

    m_FillBrush = pBrush;

    if(m_FillBrush)
    {
        AddRefObject(m_FillBrush);

        IFC(AddVisualResource(m_FillBrush, &INSTANCE_CHANGE_CALLBACK( CGeometryVisual, OnFillBrushChanged )));        
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::InternalSetStrokeBrush(
    __in CBrush* pBrush
    )
{
    HRESULT hr = S_OK;

    if(m_StrokeBrush)
    {
        IFC(RemoveVisualResource(m_StrokeBrush, &INSTANCE_CHANGE_CALLBACK( CGeometryVisual, OnStrokeBrushChanged )));

        ReleaseObject(m_StrokeBrush);
    }

    ReleaseObject(m_StrokeGraphicsBrush);

    m_StrokeBrush = pBrush;

    if(m_StrokeBrush)
    {
        IFC(AddVisualResource(m_StrokeBrush, &INSTANCE_CHANGE_CALLBACK( CGeometryVisual, OnStrokeBrushChanged )));

        AddRefObject(m_StrokeBrush);
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::InternalSetStrokeThickness(
    FLOAT Thickness
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(Thickness >= 0 );

    m_StrokeThickness = Thickness;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::OnFillBrushChanged(
    __in_opt CObjectWithType* pOldValue,
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    ReleaseObject(m_FillGraphicsBrush);

    IFC(InvalidateVisual());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CGeometryVisual::OnStrokeBrushChanged(
    __in_opt CObjectWithType* pOldValue,
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    ReleaseObject(m_StrokeGraphicsBrush);

    IFC(InvalidateVisual());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CGeometryVisual::PreRender(
    CPreRenderContext& Context
    )
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;

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
            m_UpdateStrokeTransform = TRUE;
        }
    }

    if(m_UpdateFillTransform || m_UpdateStrokeTransform)
    {
        RectF GeometryBounds;

        IFC(m_Geometry->GetBounds(&GeometryBounds));

        if(m_UpdateFillTransform && m_FillGraphicsBrush != NULL)
        {
            RectF brushBounds;

            IFC(m_FillGraphicsBrush->GetBounds(brushBounds));

            m_UpdateFillTransform = FALSE;

            {
                FLOAT GeometryWidth = GeometryBounds.right - GeometryBounds.left;
                FLOAT GeometryHeight = GeometryBounds.bottom - GeometryBounds.top;

                //TODO: Handle brush translation.
                m_FillBrushTransform = m_BrushTransform * Matrix3X2F::Scale(GeometryWidth / brushBounds.GetWidth(), GeometryHeight / brushBounds.GetHeight()) * Matrix3X2F::Translation(GeometryBounds.left, GeometryBounds.top);
            }
        }

        if(m_UpdateStrokeTransform && m_StrokeGraphicsBrush != NULL)
        {
            RectF brushBounds;

            IFC(m_StrokeGraphicsBrush->GetBounds(brushBounds));

            m_UpdateStrokeTransform = FALSE;

            {
                FLOAT GeometryWidth = GeometryBounds.right - GeometryBounds.left;
                FLOAT GeometryHeight = GeometryBounds.bottom - GeometryBounds.top;

                //TODO: This should use the stroked bounds most likely.
                //TODO: Handle brush translation.
                m_StrokeBrushTransform = m_BrushTransform * Matrix3X2F::Scale(GeometryWidth / brushBounds.GetWidth(), GeometryHeight / brushBounds.GetHeight()) * Matrix3X2F::Translation(GeometryBounds.left, GeometryBounds.top);
            }
        }
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CGeometryVisual::RenderTransformed(
    CRenderContext& Context
    )
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
            IFC(m_StrokeGraphicsBrush->SetTransform(m_StrokeBrushTransform));

            IFC(pRenderTarget->DrawGeometry(m_Geometry, m_StrokeGraphicsBrush, m_StrokeThickness));
        }
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CGeometryVisual::HitTest(
    Point2F LocalPoint, 
    __deref_out_opt CHitTestResult** ppHitTestResult
    )
{
    HRESULT hr = S_OK;
    BOOL FillContainsPoint = FALSE;

    IFCPTR(ppHitTestResult);

    if(m_Geometry != NULL)
    {
        Matrix3X2F LocalTransform;

        IFC(GetLocalTransform(&LocalTransform));

        IFC(m_Geometry->FillContainsPoint(LocalPoint * LocalTransform, &FillContainsPoint));
    }

    if(FillContainsPoint)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

Cleanup:
    return hr;
}