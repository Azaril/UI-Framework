//#include "RectangleVisual.h"
//
//CRectangleVisual::CRectangleVisual() : m_Brush(NULL),
//                                       m_GraphicsBrush(NULL),
//                                       m_OutlineBrush(NULL),
//                                       m_OutlineGraphicsBrush(NULL)
//{
//    m_Position.x = 0;
//    m_Position.y = 0;
//
//    m_Size.width = 0;
//    m_Size.height = 0;
//
//    m_BorderThickness.left = 0;
//    m_BorderThickness.top = 0;
//    m_BorderThickness.right = 0;
//    m_BorderThickness.bottom = 0;
//}
//
//CRectangleVisual::~CRectangleVisual()
//{
//    ReleaseObject(m_Brush);
//    ReleaseObject(m_GraphicsBrush);
//    ReleaseObject(m_OutlineBrush);
//    ReleaseObject(m_OutlineGraphicsBrush);
//}
//
//HRESULT CRectangleVisual::Initialize()
//{
//    HRESULT hr = S_OK;
//
//    IFC(CVisual::Initialize());
//
//Cleanup:
//    return hr;
//}
//
//HRESULT CRectangleVisual::SetBrush(CBrush* pBrush)
//{
//    HRESULT hr = S_OK;
//
//    if(m_Brush)
//    {
//        IFC(RemoveVisualResource(m_Brush));
//
//        ReleaseObject(m_Brush);
//    }
//
//    ReleaseObject(m_GraphicsBrush);
//
//    m_Brush = pBrush;
//
//    if(m_Brush)
//    {
//        m_Brush = pBrush;
//
//        AddRefObject(m_Brush);
//
//        IFC(AddVisualResource(m_Brush));
//    }
//
//Cleanup:
//    return hr;
//}
//
//HRESULT CRectangleVisual::SetOutlineBrush(CBrush* pBrush)
//{
//    HRESULT hr = S_OK;
//
//    if(m_OutlineBrush)
//    {
//        IFC(RemoveVisualResource(m_OutlineBrush));
//
//        ReleaseObject(m_OutlineBrush);
//    }
//
//    ReleaseObject(m_OutlineGraphicsBrush);
//
//    m_OutlineBrush = pBrush;
//
//    if(m_OutlineBrush)
//    {
//        m_OutlineBrush = pBrush;
//
//        AddRefObject(m_OutlineBrush);
//
//        IFC(AddVisualResource(m_OutlineBrush));
//    }
//
//Cleanup:
//    return hr;
//}
//
//HRESULT CRectangleVisual::SetSize(SizeF Size)
//{
//    HRESULT hr = S_OK;
//
//    m_Size = Size;
//
//    return hr;
//}
//
//HRESULT CRectangleVisual::SetPosition(Point2F Position)
//{
//    HRESULT hr = S_OK;
//
//    m_Position = Position;
//
//    return hr;
//}
//
//HRESULT CRectangleVisual::SetBorderThickness(RectF Thickness)
//{
//    HRESULT hr = S_OK;
//
//    m_BorderThickness = Thickness;
//
//    return hr;
//}
//
//HRESULT CRectangleVisual::PreRender(CPreRenderContext& Context)
//{
//    HRESULT hr = S_OK;
//    CRenderTarget* pRenderTarget = NULL;
//
//    pRenderTarget = Context.GetRenderTarget();
//    IFCPTR(pRenderTarget);
//
//    if(m_Brush != NULL && m_GraphicsBrush == NULL)
//    {
//        if(FAILED(m_Brush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &m_GraphicsBrush)))
//        {
//            m_GraphicsBrush = NULL;
//        }
//    }
//
//    if(m_OutlineBrush != NULL && m_OutlineGraphicsBrush == NULL)
//    {
//        if(FAILED(m_OutlineBrush->GetGraphicsBrush(m_VisualContext.GetGraphicsDevice(), pRenderTarget, &m_OutlineGraphicsBrush)))
//        {
//            m_OutlineGraphicsBrush = NULL;
//        }
//    }
//
//Cleanup:
//    return hr;
//}
//
//HRESULT CRectangleVisual::RenderTransformed(CRenderContext& Context)
//{
//    HRESULT hr = S_OK;
//    CRenderTarget* pRenderTarget = NULL;
//
//    RectF Rect = { m_Position.x, m_Position.y, m_Position.x + m_Size.width, m_Position.y + m_Size.height };
//
//    pRenderTarget = Context.GetRenderTarget();
//    IFCPTR(pRenderTarget);    
//
//    if(m_GraphicsBrush != NULL)
//    {
//        IFC(pRenderTarget->FillRectangle(Rect, m_GraphicsBrush));
//    }
//
//    if(m_OutlineGraphicsBrush != NULL)
//    {
//        IFC(pRenderTarget->DrawRectangle(Rect, m_OutlineGraphicsBrush));
//    }
//
//    IFC(CVisual::RenderTransformed(Context));
//
//Cleanup:
//    return hr;
//}