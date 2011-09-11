#include "OpenGLES20RenderTarget.h"

COpenGLES20RenderTarget::COpenGLES20RenderTarget(
	) 
	: m_RenderBuffer(0)
	, m_FrameBuffer(0)
	, m_pContext(NULL)
	, m_Width(0)
	, m_Height(0)
{
}

COpenGLES20RenderTarget::~COpenGLES20RenderTarget(
	)
{
	IGNOREHR(ApplyContext());
	
	if (m_FrameBuffer != 0)
	{
		glDeleteFramebuffers(1, &m_FrameBuffer);
	}

	if (m_RenderBuffer != 0)
	{
		glDeleteRenderbuffers(1, &m_RenderBuffer);
	}
}

__checkReturn HRESULT
COpenGLES20RenderTarget::Initialize(
	__in GLuint RenderBuffer,
	__in GLuint FrameBuffer,
	__in_opt COpenGLES20Context* pContext
	)
{
    HRESULT hr = S_OK;

	m_RenderBuffer = RenderBuffer;
	m_FrameBuffer = FrameBuffer;

	m_pContext = pContext;

	IFC(ApplyContext());

	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_Width);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_Height);

Cleanup:
    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::ApplyContext(
	)
{
	HRESULT hr = S_OK;

	if (m_pContext != NULL)
	{
		IFCEXPECT(m_pContext->Apply());
	}

Cleanup:
	return hr;
}

GLuint
COpenGLES20RenderTarget::GetRenderBuffer(
    )
{
    return m_RenderBuffer;
}

SizeF 
COpenGLES20RenderTarget::GetSize(
	)
{
    return SizeF((FLOAT)m_Width, (FLOAT)m_Height);
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::BeginRendering(
	)
{
    HRESULT hr = S_OK;

    IFC(ApplyContext());

    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    //TODO: Is this needed? Does the render buffer stay attached?
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RenderBuffer);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::EndRendering(
	)
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::SetTransform(
	const Matrix3X2F& Transform
	)
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::Clear(
	const ColorF& Color
	)
{
    HRESULT hr = S_OK;

    glClearColor(Color.r, Color.g, Color.b, Color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::CreateSolidBrush(
	const ColorF& Color, 
	__deref_out CGraphicsBrush** ppBrush
	)
{
    HRESULT hr = S_OK;
//     ID2D1SolidColorBrush* pD2DSolidColorBrush = NULL;
//     CD2DSolidColorBrush* pSolidBrush = NULL;

//     IFCPTR(ppBrush);

//     IFC(m_RenderTarget->CreateSolidColorBrush(Color, &pD2DSolidColorBrush));

//     IFC(CD2DSolidColorBrush::Create(pD2DSolidColorBrush, &pSolidBrush));

//     *ppBrush = pSolidBrush;
//     pSolidBrush = NULL;

// Cleanup:
//     ReleaseObject(pD2DSolidColorBrush);
//     ReleaseObject(pSolidBrush);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::CreateLinearGradientBrush(
	const Point2F& StartPoint, 
	const Point2F& EndPoint, 
	__in_ecount(GradientStopCount) const GradientStop* pGradientStops, 
	UINT32 GradientStopCount, 
	__deref_out CGraphicsBrush** ppBrush
	)
{
    HRESULT hr = S_OK;
//     D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES BrushProperties = { 0 };
//     ID2D1GradientStopCollection* pD2DGradientStops = NULL;
//     ID2D1LinearGradientBrush* pD2DLinearGradientBrush = NULL;
//     CD2DLinearGradientBrush* pLinearGradientBrush = NULL;

//     IFCPTR(ppBrush);

//     BrushProperties.startPoint = StartPoint;
//     BrushProperties.endPoint = EndPoint;

//     IFC(m_RenderTarget->CreateGradientStopCollection(pGradientStops, GradientStopCount, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pD2DGradientStops));

//     IFC(m_RenderTarget->CreateLinearGradientBrush(BrushProperties, pD2DGradientStops, &pD2DLinearGradientBrush));

//     IFC(CD2DLinearGradientBrush::Create(pD2DLinearGradientBrush, &pLinearGradientBrush));

//     *ppBrush = pLinearGradientBrush;
//     pLinearGradientBrush = NULL;

// Cleanup:
//     ReleaseObject(pD2DGradientStops);
//     ReleaseObject(pD2DLinearGradientBrush);
//     ReleaseObject(pLinearGradientBrush);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::GetDefaultBrush(
	DefaultBrush::Value Type, 
	__deref_out CGraphicsBrush** ppBrush
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppBrush);

    //TODO: Move lookup to a common class and precreate brushes.
    switch(Type)
    {
        case DefaultBrush::TextForeground:
            {
                IFC(CreateSolidBrush(ColorF(Color::Black), ppBrush));
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

__checkReturn HRESULT 
COpenGLES20RenderTarget::DrawRectangle(
	const RectF& Size, 
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;
//     CD2DBrush* pD2DBrush = NULL;

//     IFCPTR(pBrush);

//     pD2DBrush = (CD2DBrush*)pBrush;

//     m_RenderTarget->DrawRectangle(Size, pD2DBrush->GetD2DBrush(), 0, NULL);

// Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::FillRectangle(
	const RectF& Size,
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;
//     CD2DBrush* pD2DBrush = NULL;

//     IFCPTR(pBrush);

//     pD2DBrush = (CD2DBrush*)pBrush;

//     m_RenderTarget->FillRectangle(Size, pD2DBrush->GetD2DBrush());

// Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::RenderTextLayout(
	const Point2F& Origin, 
	__in const CTextLayout* pTextLayout, 
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;
//     CD2DBrush* pD2DBrush = NULL;
//     IDWriteTextLayout* pInternalTextLayout = NULL;

//     IFCPTR(pTextLayout);
//     IFCPTR(pBrush);

//     pD2DBrush = (CD2DBrush*)pBrush;

//     switch(pTextLayout->GetType())
//     {
//         case TypeIndex::TextLayout:
//             {
//                 CDirectWriteTextLayout* pDWLayout = (CDirectWriteTextLayout*)pTextLayout;

//                 IFC(pDWLayout->GetDirectWriteTextLayout(&pInternalTextLayout))

//                 break;
//             }

//         case TypeIndex::EditableTextLayout:
//             {
//                 CDirectWriteEditableTextLayout* pDWLayout = (CDirectWriteEditableTextLayout*)pTextLayout;

//                 IFC(pDWLayout->GetDirectWriteTextLayout(&pInternalTextLayout))

//                 break;
//             }

//         default:
//             {
//                 IFC(E_UNEXPECTED);
//             }
//     }

//     m_RenderTarget->DrawTextLayout(Origin, pInternalTextLayout, pD2DBrush->GetD2DBrush());

// Cleanup:
//     ReleaseObject(pInternalTextLayout);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::LoadBitmap(
	__in const CBitmapSource* pSource, 
	__deref_out CBitmap** ppBitmap
	)
{
    HRESULT hr = S_OK;
//     CWICBitmapSource* pWICBitmapSource = NULL;
//     ID2D1Bitmap* pD2DBitmap = NULL;
//     CD2DBitmap* pBitmap = NULL;

//     IFCPTR_NOTRACE(pSource);
//     IFCPTR(ppBitmap);

//     pWICBitmapSource = (CWICBitmapSource*)pSource;

//     IFC(m_RenderTarget->CreateBitmapFromWicBitmap(pWICBitmapSource->GetWICBitmapSource(), &pD2DBitmap));

//     IFC(CD2DBitmap::Create(pD2DBitmap, &pBitmap));

//     *ppBitmap = pBitmap;
//     pBitmap = NULL;

// Cleanup:
//     ReleaseObject(pD2DBitmap);
//     ReleaseObject(pBitmap);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::CreateBitmapBrush(
	__in const CBitmap* pBitmap, 
	__deref_out CGraphicsBrush** pBrush
	)
{
    HRESULT hr = S_OK;
//     CD2DBitmap* pD2DBitmap = NULL;
//     CD2DBitmapBrush* pD2DBrush = NULL;
//     ID2D1BitmapBrush* pD2DBitmapBrush = NULL;
//     D2D1_BITMAP_BRUSH_PROPERTIES BrushProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

//     IFCPTR(pBitmap);
//     IFCPTR(pBrush);

//     pD2DBitmap = (CD2DBitmap*)pBitmap;

//     IFC(m_RenderTarget->CreateBitmapBrush(pD2DBitmap->GetD2DBitmap(), BrushProperties, &pD2DBitmapBrush));

//     IFC(CD2DBitmapBrush::Create(pD2DBitmapBrush, &pD2DBrush));

//     *pBrush = pD2DBrush;
//     pD2DBrush = NULL;

// Cleanup:
//     ReleaseObject(pD2DBitmapBrush);
//     ReleaseObject(pD2DBrush);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::FillGeometry(
	__in const CGeometry* pGeometry,
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;
    // CD2DBrush* pD2DBrush = NULL;

    // IFCPTR(pGeometry);
    // IFCPTR(pBrush);

    // pD2DBrush = (CD2DBrush*)pBrush;

    // switch(pGeometry->GetType())
    // {
    //     case TypeIndex::RectangleGeometry:
    //         {
    //             CD2DRectangleGeometry* pRectangleGeometry = (CD2DRectangleGeometry*)pGeometry;

    //             m_RenderTarget->FillGeometry(pRectangleGeometry->GetD2DGeometry(), pD2DBrush->GetD2DBrush());

    //             break;
    //         }

    //     case TypeIndex::RoundedRectangleGeometry:
    //         {
    //             CD2DRoundedRectangleGeometry* pRoundedRectangleGeometry = (CD2DRoundedRectangleGeometry*)pGeometry;

    //             m_RenderTarget->FillGeometry(pRoundedRectangleGeometry->GetD2DGeometry(), pD2DBrush->GetD2DBrush());

    //             break;
    //         }

    //     default:
    //         {
    //             IFC(E_UNEXPECTED);
    //         }
    // }

// Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::DrawGeometry(
	__in const CGeometry* pGeometry, 
	__in const CGraphicsBrush* pBrush, 
	FLOAT StrokeThickness
	)
{
    HRESULT hr = S_OK;
//     CD2DBrush* pD2DBrush = NULL;
//     ID2D1Geometry* pD2DGeometry = NULL;

//     IFCPTR(pGeometry);
//     IFCPTR(pBrush);

//     pD2DBrush = (CD2DBrush*)pBrush;

//     IFC(UnwrapGeometry(pGeometry, &pD2DGeometry));

//     m_RenderTarget->DrawGeometry(pD2DGeometry, pD2DBrush->GetD2DBrush(), StrokeThickness);

// Cleanup:
//     ReleaseObject(pD2DGeometry);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::CreateLayer(
	__deref_out CLayer** ppLayer
	)
{
    HRESULT hr = S_OK;
//     ID2D1Layer* pD2DLayer = NULL;
//     CD2DLayer* pLayer = NULL;

//     IFC(m_RenderTarget->CreateLayer(&pD2DLayer));

//     IFC(CD2DLayer::Create(pD2DLayer, &pLayer));

//     *ppLayer = pLayer;
//     pLayer = NULL;

// Cleanup:
//     ReleaseObject(pD2DLayer);
//     ReleaseObject(pLayer);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::PushLayer(
	__in const CLayer* pLayer, 
	const RectF& ClippingRect,
	FLOAT Opacity, 
	__in_opt const CGeometry* pClippingGeometry
	)
{
    HRESULT hr = S_OK;
//     CD2DLayer* pD2DLayer = NULL;
//     ID2D1Geometry* pD2DClipGeometry = NULL;

//     IFCPTR(pLayer);

//     pD2DLayer = (CD2DLayer*)pLayer;

//     if(pClippingGeometry)
//     {
//         IFC(UnwrapGeometry(pClippingGeometry, &pD2DClipGeometry));
//     }

//     m_RenderTarget->PushLayer(D2D1::LayerParameters(ClippingRect, pD2DClipGeometry, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, D2D1::IdentityMatrix(), Opacity), pD2DLayer->GetLayer());

// Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::PopLayer(
	)
{
    HRESULT hr = S_OK;

    //m_RenderTarget->PopLayer();

    return hr;
}