#include "D2DRenderTarget.h"
#include "D2DSolidColorBrush.h"
#include "D2DBitmap.h"
#include "D2DBitmapBrush.h"
#include "D2DRectangleGeometry.h"
#include "D2DRoundedRectangleGeometry.h"
#include "ErrorChecking.h"
#include "DirectWriteTextLayout.h"
#include "WICBitmapSource.h"

CD2DRenderTarget::CD2DRenderTarget() : m_RenderTarget(NULL)
{
}

CD2DRenderTarget::~CD2DRenderTarget()
{
    ReleaseObject(m_RenderTarget);
}

HRESULT CD2DRenderTarget::Initialize(ID2D1RenderTarget* pRenderTarget)
{
    HRESULT hr = S_OK;

    IFCPTR(pRenderTarget);

    m_RenderTarget = pRenderTarget;
    AddRefObject(m_RenderTarget);

Cleanup:
    return hr;
}

SizeF CD2DRenderTarget::GetSize()
{
    return m_RenderTarget->GetSize();
}

HRESULT CD2DRenderTarget::BeginRendering()
{
    HRESULT hr = S_OK;

    m_RenderTarget->BeginDraw();

    return hr;
}

HRESULT CD2DRenderTarget::EndRendering()
{
    HRESULT hr = S_OK;

    IFC(m_RenderTarget->EndDraw());

Cleanup:
    return hr;
}

HRESULT CD2DRenderTarget::SetTransform( const Matrix3X2& Transform )
{
    HRESULT hr = S_OK;

    m_RenderTarget->SetTransform(Transform);

    return hr;
}

HRESULT CD2DRenderTarget::Clear(ColorF Color)
{
    HRESULT hr = S_OK;

    m_RenderTarget->Clear(Color);

    return hr;
}

HRESULT CD2DRenderTarget::CreateSolidBrush(ColorF Color, CGraphicsBrush** ppBrush)
{
    HRESULT hr = S_OK;
    ID2D1SolidColorBrush* pD2DSolidColorBrush = NULL;
    CD2DSolidColorBrush* pSolidBrush = NULL;

    IFCPTR(ppBrush);

    IFC(m_RenderTarget->CreateSolidColorBrush(Color, &pD2DSolidColorBrush));

    IFC(CD2DSolidColorBrush::Create(pD2DSolidColorBrush, &pSolidBrush));

    *ppBrush = pSolidBrush;
    pSolidBrush = NULL;

Cleanup:
    ReleaseObject(pD2DSolidColorBrush);
    ReleaseObject(pSolidBrush);

    return hr;
}

HRESULT CD2DRenderTarget::GetDefaultBrush(DefaultBrush::Value Type, CGraphicsBrush** ppBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(ppBrush);

    //TODO: Move lookup to a common class and precreate brushes.
    switch(Type)
    {
        case DefaultBrush::TextForeground:
            {
                IFC(CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::Black), ppBrush));
                break;
            }

        default:
            {
                IFC(E_FAIL);
            }
    }

Cleanup:
    return hr;
}

HRESULT CD2DRenderTarget::DrawRectangle(const RectF& Size, CGraphicsBrush* pBrush)
{
    HRESULT hr = S_OK;
    CD2DBrush* pD2DBrush = NULL;

    IFCPTR(pBrush);

    pD2DBrush = (CD2DBrush*)pBrush;

    m_RenderTarget->DrawRectangle(Size, pD2DBrush->GetD2DBrush(), 0, NULL);

Cleanup:
    return hr;
}

HRESULT CD2DRenderTarget::FillRectangle(const RectF& Size, CGraphicsBrush* pBrush)
{
    HRESULT hr = S_OK;
    CD2DBrush* pD2DBrush = NULL;

    IFCPTR(pBrush);

    pD2DBrush = (CD2DBrush*)pBrush;

    m_RenderTarget->FillRectangle(Size, pD2DBrush->GetD2DBrush());

Cleanup:
    return hr;
}

//HRESULT CD2DRenderTarget::CreateCompatibleRenderTarget()
//{
    //HRESULT hr = S_OK;

//Cleanup:
    //return hr;
//}

HRESULT CD2DRenderTarget::RenderTextLayout(const Point2F& Origin, CTextLayout* pTextLayout, CGraphicsBrush* pBrush)
{
    HRESULT hr = S_OK;
    CD2DBrush* pD2DBrush = NULL;
    CDirectWriteTextLayout* pDirectWriteTextLayout = NULL;

    IFCPTR(pTextLayout);
    IFCPTR(pBrush);

    pD2DBrush = (CD2DBrush*)pBrush;
    pDirectWriteTextLayout = (CDirectWriteTextLayout*)pTextLayout;

    m_RenderTarget->DrawTextLayout(Origin, pDirectWriteTextLayout->GetDirectWriteTextLayout(), pD2DBrush->GetD2DBrush());

Cleanup:
    return hr;
}

HRESULT CD2DRenderTarget::LoadBitmap(CBitmapSource* pSource, CBitmap** ppBitmap)
{
    HRESULT hr = S_OK;
    CWICBitmapSource* pWICBitmapSource = NULL;
    ID2D1Bitmap* pD2DBitmap = NULL;
    CD2DBitmap* pBitmap = NULL;

    IFCPTR(pSource);
    IFCPTR(ppBitmap);

    pWICBitmapSource = (CWICBitmapSource*)pSource;

    IFC(m_RenderTarget->CreateBitmapFromWicBitmap(pWICBitmapSource->GetWICBitmapSource(), &pD2DBitmap));

    IFC(CD2DBitmap::Create(pD2DBitmap, &pBitmap));

    *ppBitmap = pBitmap;
    pBitmap = NULL;

Cleanup:
    ReleaseObject(pD2DBitmap);
    ReleaseObject(pBitmap);

    return hr;
}

HRESULT CD2DRenderTarget::CreateBitmapBrush(CBitmap* pBitmap, CGraphicsBrush** pBrush)
{
    HRESULT hr = S_OK;
    CD2DBitmap* pD2DBitmap = NULL;
    CD2DBitmapBrush* pD2DBrush = NULL;
    ID2D1BitmapBrush* pD2DBitmapBrush = NULL;
    D2D1_BITMAP_BRUSH_PROPERTIES BrushProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

    IFCPTR(pBitmap);
    IFCPTR(pBrush);

    pD2DBitmap = (CD2DBitmap*)pBitmap;

    IFC(m_RenderTarget->CreateBitmapBrush(pD2DBitmap->GetD2DBitmap(), BrushProperties, &pD2DBitmapBrush));

    IFC(CD2DBitmapBrush::Create(pD2DBitmapBrush, &pD2DBrush));

    *pBrush = pD2DBrush;
    pD2DBrush = NULL;

Cleanup:
    ReleaseObject(pD2DBitmapBrush);
    ReleaseObject(pD2DBrush);

    return hr;
}

HRESULT CD2DRenderTarget::FillGeometry(CGeometry* pGeometry, CGraphicsBrush* pBrush)
{
    HRESULT hr = S_OK;
    CD2DBrush* pD2DBrush = NULL;

    IFCPTR(pGeometry);
    IFCPTR(pBrush);

    pD2DBrush = (CD2DBrush*)pBrush;

    switch(pGeometry->GetType())
    {
        case TypeIndex::RectangleGeometry:
            {
                CD2DRectangleGeometry* pRectangleGeometry = (CD2DRectangleGeometry*)pGeometry;

                m_RenderTarget->FillGeometry(pRectangleGeometry->GetD2DGeometry(), pD2DBrush->GetD2DBrush());

                break;
            }

        case TypeIndex::RoundedRectangleGeometry:
            {
                CD2DRoundedRectangleGeometry* pRoundedRectangleGeometry = (CD2DRoundedRectangleGeometry*)pGeometry;

                m_RenderTarget->FillGeometry(pRoundedRectangleGeometry->GetD2DGeometry(), pD2DBrush->GetD2DBrush());

                break;
            }

        default:
            {
                IFC(E_FAIL);
            }
    }

Cleanup:
    return hr;
}

HRESULT CD2DRenderTarget::DrawGeometry(CGeometry* pGeometry, CGraphicsBrush* pBrush, FLOAT StrokeThickness)
{
    HRESULT hr = S_OK;
    CD2DBrush* pD2DBrush = NULL;

    IFCPTR(pGeometry);
    IFCPTR(pBrush);

    pD2DBrush = (CD2DBrush*)pBrush;

    switch(pGeometry->GetType())
    {
        case TypeIndex::RectangleGeometry:
            {
                CD2DRectangleGeometry* pRectangleGeometry = (CD2DRectangleGeometry*)pGeometry;

                m_RenderTarget->DrawGeometry(pRectangleGeometry->GetD2DGeometry(), pD2DBrush->GetD2DBrush(), StrokeThickness);

                break;
            }

        case TypeIndex::RoundedRectangleGeometry:
            {
                CD2DRoundedRectangleGeometry* pRoundedRectangleGeometry = (CD2DRoundedRectangleGeometry*)pGeometry;

                m_RenderTarget->DrawGeometry(pRoundedRectangleGeometry->GetD2DGeometry(), pD2DBrush->GetD2DBrush(), StrokeThickness);

                break;
            }

        default:
            {
                IFC(E_FAIL);
            }
    }

Cleanup:
    return hr;
}