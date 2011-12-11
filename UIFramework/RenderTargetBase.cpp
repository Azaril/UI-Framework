#include "RenderTargetBase.h"
#include "CoreRectangleGeometry.h"
#include "CoreRoundedRectangleGeometry.h"
#include "CorePathGraphicsGeometry.h"
#include "StaticTesselator.h"
#include "LinearGradientBrushBase.h"
#include "SolidColorBrushBase.h"
#include "BitmapSourceBase.h"
#include "BitmapBase.h"
#include "BitmapBrushBase.h"
#include "TextLayoutBase.h"
#include "EditableTextLayoutBase.h"
#include <algorithm>

CRenderTargetBase::CRenderTargetBase(
	) 
    : m_Transform(Matrix3X2F::Identity())
    , m_pTesselationSink(NULL)
    , m_pTextureAtlasPool(NULL)
    , m_pLastRenderedTextureAtlas(NULL)
    , m_pLastRenderedMaskAtlas(NULL)
    , m_pDefaultWhitePixelTexture(NULL)
{
}

CRenderTargetBase::~CRenderTargetBase(
	)
{
	IGNOREHR(ApplyContext());
    
    ReleaseObject(m_pDefaultWhitePixelTexture);
    ReleaseObject(m_pLastRenderedTextureAtlas);
    ReleaseObject(m_pLastRenderedMaskAtlas);
    
    ReleaseObject(m_pTesselationSink);
    
    ReleaseObject(m_pTextureAtlasPool);
}

__checkReturn HRESULT
CRenderTargetBase::Initialize(
	__in CGeometryTesselationSink* pTesselationSink,
    __in ITextureAtlasPool* pTextureAtlasPool,
    __in CTextureAtlasView* pWhitePixelTexture
	)
{
    HRESULT hr = S_OK;

	SetObject(m_pTesselationSink, pTesselationSink);
    SetObject(m_pTextureAtlasPool, pTextureAtlasPool);
    SetObject(m_pDefaultWhitePixelTexture, pWhitePixelTexture);
    
    return hr;
}

__checkReturn HRESULT
CRenderTargetBase::ApplyContext( 
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::BeginRendering(
	)
{
    HRESULT hr = S_OK;

    ReplaceObject(m_pLastRenderedTextureAtlas, (ITextureAtlasWithWhitePixel*)NULL);
    ReplaceObject(m_pLastRenderedMaskAtlas, (ITextureAtlasWithWhitePixel*)NULL);

    IFC(ApplyContext());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::EndRendering(
	)
{
    HRESULT hr = S_OK;

    IFC(Flush());
   
    ReplaceObject(m_pLastRenderedTextureAtlas, (ITextureAtlasWithWhitePixel*)NULL);
    ReplaceObject(m_pLastRenderedMaskAtlas, (ITextureAtlasWithWhitePixel*)NULL);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::SetTransform(
	const Matrix3X2F& Transform
	)
{
    HRESULT hr = S_OK;
    
    m_Transform = Transform;

    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::CreateSolidBrush(
	const ColorF& Color, 
	__deref_out CGraphicsBrush** ppBrush
	)
{
    HRESULT hr = S_OK;
    CSolidColorBrushBase* pSolidBrush = NULL;

     IFCPTR(ppBrush);

     IFC(CSolidColorBrushBase::Create(Color, &pSolidBrush));

     *ppBrush = pSolidBrush;
     pSolidBrush = NULL;

 Cleanup:
     ReleaseObject(pSolidBrush);

    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::CreateLinearGradientBrush(
	const Point2F& StartPoint, 
	const Point2F& EndPoint, 
	__in_ecount(GradientStopCount) const GradientStop* pGradientStops, 
	UINT32 GradientStopCount, 
	__deref_out CGraphicsBrush** ppBrush
	)
{
    HRESULT hr = S_OK;
    ITexture* pGradientTexture = NULL;
    CLinearGradientBrushBase* pGradientBrush = NULL;
    GradientStop* pSortedGradientStops = NULL;
    BYTE* pColorBuffer = NULL;
    UINT32 colorBufferSize = 0;    
    PixelFormat::Value format = PixelFormat::Unknown;
    //TODO: Dynamically figure out how large this should be.
    UINT32 textureLength = 256 - 2;
    Matrix3X2F textureToBrushTransform;
    Point2F directionPoint;
    
    IFCEXPECT(GradientStopCount > 0);
    
    //
    // Allocate space for sorting gradient stops.
    //
    pSortedGradientStops = new GradientStop[GradientStopCount];
    IFCOOM(pSortedGradientStops);
    
    //
    // Copy and sort the original gradient stops.
    //
    memcpy(pSortedGradientStops, pGradientStops, sizeof(GradientStop) * GradientStopCount);
    
    std::stable_sort(pSortedGradientStops, pSortedGradientStops + GradientStopCount);
    
    //
    // Allocate the gradient texture.
    //
    IFC(m_pTextureAtlasPool->AllocateTexture(textureLength, 1, &pGradientTexture));
    
    format = pGradientTexture->GetPixelFormat();
    IFCEXPECT(format != PixelFormat::Unknown);
    
    colorBufferSize = PixelFormat::GetLineSize(format, textureLength);
    IFCEXPECT(colorBufferSize > 0);
    
    pColorBuffer = new BYTE[colorBufferSize];
    IFCOOM(pColorBuffer);
    
    //
    // Calculate gradient direction (perpendicular to gradient).
    //
    //directionPoint.x = -(EndPoint.y - StartPoint.y) + StartPoint.x;
    //directionPoint.y = (EndPoint.x - StartPoint.x) + StartPoint.y;
    
    //
    // Calculate the transform.
    //
    if (StartPoint != EndPoint)
    {
        Point2F direction = (EndPoint - StartPoint).Normalize();
        FLOAT rotationAngle = (FLOAT)atan2(direction.y, direction.x) * (180.0f / 3.1415927f);
        FLOAT scaleX = direction.x != 0.0f ? 1.0f / fabs(direction.x) : 1.0f;
        FLOAT scaleY = direction.y != 0.0f ? 1.0f / fabs(direction.y) : 1.0f; 
        
        textureToBrushTransform = Matrix3X2F::Rotation(rotationAngle) * Matrix3X2F::Translation(StartPoint.x, StartPoint.y) * Matrix3X2F::Scale(scaleX, scaleY);
    }
    else
    {        
        textureToBrushTransform = Matrix3X2F::Scale(0, 0);
    }
    
    //
    // Generate the gradient texture.
    //
    {
        FLOAT step = 1 / (FLOAT)(textureLength - 1);
        INT32 fromColorIndex = 0;
        BYTE* pWriteOffset = pColorBuffer;
        
        for (UINT32 i = 0; i < textureLength; ++i)
        {
            ColorF texelColor;
            
            FLOAT offset = step * (FLOAT)i;
            
            while (fromColorIndex < (INT32)GradientStopCount - 2)
            {
                if (offset >= pSortedGradientStops[fromColorIndex + 1].position)
                {
                    ++fromColorIndex;
                }
                else
                {
                    break;
                }
            }            
            
            if (offset <= pSortedGradientStops[fromColorIndex].position)
            {
                texelColor = ColorF(pSortedGradientStops[fromColorIndex].color);
            }
            else if(offset >= pSortedGradientStops[fromColorIndex + 1].position)
            {
                texelColor = ColorF(pSortedGradientStops[fromColorIndex + 1].color);
            }
            else
            {
                FLOAT interpolateVal = (offset - pSortedGradientStops[fromColorIndex].position) / (pSortedGradientStops[fromColorIndex + 1].position - pSortedGradientStops[fromColorIndex].position);
                
                texelColor = ColorF::Interpolate(ColorF(pSortedGradientStops[fromColorIndex].color), ColorF(pSortedGradientStops[fromColorIndex + 1].color), interpolateVal);
            }
            
            switch(format)
            {
                case PixelFormat::B8G8R8A8:
                    {
                        *pWriteOffset = (BYTE)(texelColor.b * 255.0f);
                        ++pWriteOffset;

                        *pWriteOffset = (BYTE)(texelColor.g * 255.0f);
                        ++pWriteOffset;

                        *pWriteOffset = (BYTE)(texelColor.r * 255.0f);
                        ++pWriteOffset;

                        *pWriteOffset = (BYTE)(texelColor.a * 255.0f);
                        ++pWriteOffset;

                        break;
                    }

                case PixelFormat::R8G8B8A8:
                    {
                        *pWriteOffset = (BYTE)(texelColor.r * 255.0f);
                        ++pWriteOffset;

                        *pWriteOffset = (BYTE)(texelColor.g * 255.0f);
                        ++pWriteOffset;

                        *pWriteOffset = (BYTE)(texelColor.b * 255.0f);
                        ++pWriteOffset;

                        *pWriteOffset = (BYTE)(texelColor.a * 255.0f);
                        ++pWriteOffset;

                        break;
                    }
                    
                default:
                    {
                        IFC(E_UNEXPECTED);
                    }
            }
        }
    }
    
    IFC(pGradientTexture->SetData(pColorBuffer, colorBufferSize, colorBufferSize));
        
    IFC(CLinearGradientBrushBase::Create(pGradientTexture, textureToBrushTransform, &pGradientBrush));        
    
    *ppBrush = pGradientBrush;
    pGradientBrush = NULL;

Cleanup:
    ReleaseObject(pGradientTexture);
    ReleaseObject(pGradientBrush);
    delete [] pSortedGradientStops;
    delete [] pColorBuffer;
    
    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::GetDefaultBrush(
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
CRenderTargetBase::DrawRectangle(
	const RectF& Size, 
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;  
    
    IFC(m_pTesselationSink->SetTransform(m_Transform));
    
    IFC(ApplyBrush((const CGraphicsBrushBase*)pBrush));
    IFC(ApplyMask(NULL));
    
    //TODO: This should probably have stroke width...
    IFC(StaticTesselator::TesselateRectangleStroke(Size, 1.0f, m_pTesselationSink));
    
Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::FillRectangle(
	const RectF& Size,
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK; 
    
    IFC(m_pTesselationSink->SetTransform(m_Transform));
    
    IFC(ApplyBrush((const CGraphicsBrushBase*)pBrush));
    IFC(ApplyMask(NULL));
    
    IFC(StaticTesselator::TesselateRectangle(Size, m_pTesselationSink));
    
Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::RenderTextLayout(
	__in const CTextLayout* pTextLayout, 
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;
    CTextLayoutBaseCommon* pBaseLayout = NULL;
    const CGraphicsBrushBase* pGraphicsBrush = NULL;

    pGraphicsBrush = (const CGraphicsBrushBase*)pBrush;

    IFC(ApplyBrush(pGraphicsBrush));

    switch(pTextLayout->GetType())
    {
        case TypeIndex::EditableTextLayout:
            {
                pBaseLayout = (CEditableTextLayoutBase*)pTextLayout;
                break;
            }

        case TypeIndex::TextLayout:
            {
                pBaseLayout = (CTextLayoutBase*)pTextLayout;
                break;
            }

        default:
            {
                IFC(E_UNEXPECTED);
            }
    }

    m_pTesselationSink->SetTransform(m_Transform);

    IFC(pBaseLayout->Render(this, (void*)pBrush));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CRenderTargetBase::RenderGlyphRun(
    __in ITexture* pTexture,
    __in GlyphRun* pGlyphRun,
    __in_opt void* pContext
    )
{
    HRESULT hr = S_OK;

    IFC(ApplyMask(pTexture));

    {
        SizeF textureSize((FLOAT)pTexture->GetWidth(), (FLOAT)pTexture->GetHeight());

        for (list< GlyphData >::iterator it = pGlyphRun->Glyphs.begin(); it != pGlyphRun->Glyphs.end(); ++it)
        {
            RectF glyphRect = MakeRect(Point2F(it->Position.x, it->Position.y), textureSize);

            IFC(m_pTesselationSink->AddRectangleWithUnitMask(glyphRect));
        }
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::LoadBitmap(
	__in const CBitmapSource* pSource, 
	__deref_out CBitmap** ppBitmap
	)
{
    HRESULT hr = S_OK;
    CBitmapSourceBase* pBaseSource = NULL;
    ITexture* pImageTexture = NULL;
    CBitmapBase* pLoadedBitmap = NULL;
    SizeU imageSize;

    pBaseSource = (CBitmapSourceBase*)pSource;

    IFC(pBaseSource->GetSize(&imageSize));

    IFC(m_pTextureAtlasPool->AllocateTexture(imageSize.width, imageSize.height, &pImageTexture));

    IFC(pBaseSource->LoadIntoTexture(pImageTexture));

    IFC(CBitmapBase::Create(pImageTexture, &pLoadedBitmap));

    *ppBitmap = pLoadedBitmap;
    pLoadedBitmap = NULL;

Cleanup:
    ReleaseObject(pImageTexture);
    ReleaseObject(pLoadedBitmap);

    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::CreateBitmapBrush(
	__in const CBitmap* pBitmap, 
	__deref_out CGraphicsBrush** ppBrush
	)
{
    HRESULT hr = S_OK;
    CBitmapBase* pBitmapBase = NULL;
    CBitmapBrushBase* pBitmapBrush = NULL;

    pBitmapBase = (CBitmapBase*)pBitmap;

    IFC(CBitmapBrushBase::Create(pBitmapBase->GetTexture(), &pBitmapBrush));

    *ppBrush = pBitmapBrush;
    pBitmapBrush = NULL;

Cleanup:
    ReleaseObject(pBitmapBrush);

    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::FillGeometry(
	__in const CGraphicsGeometry* pGeometry,
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;
    ICoreGeometry* pCoreGeometry = NULL;

    switch(pGeometry->GetType())
    {
        case TypeIndex::RectangleGraphicsGeometry:
            {
                pCoreGeometry = (CCoreRectangleGeometry*)pGeometry;  
                    
                break;
            }

        case TypeIndex::RoundedRectangleGraphicsGeometry:
            {
                pCoreGeometry = (CCoreRoundedRectangleGeometry*)pGeometry;

                break;
            }

        default:
            {
                IFC(E_UNEXPECTED);
            }
    }
    
    IFC(m_pTesselationSink->SetTransform(m_Transform));
    
    IFC(ApplyBrush((const CGraphicsBrushBase*)pBrush));
    IFC(ApplyMask(NULL));
    
    IFC(pCoreGeometry->TesselateFill(m_pTesselationSink)); 

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::DrawGeometry(
	__in const CGraphicsGeometry* pGeometry, 
	__in const CGraphicsBrush* pBrush, 
	FLOAT StrokeThickness
	)
{
    HRESULT hr = S_OK;
    ICoreGeometry* pCoreGeometry = NULL;
       
    switch(pGeometry->GetType())
    {
        case TypeIndex::RectangleGraphicsGeometry:
        {
            pCoreGeometry = (CCoreRectangleGeometry*)pGeometry;  
            
            break;
        }
            
        case TypeIndex::RoundedRectangleGraphicsGeometry:
        {
            pCoreGeometry = (CCoreRoundedRectangleGeometry*)pGeometry;
            
            break;
        }
            
        default:
        {
            IFC(E_UNEXPECTED);
        }
    }
    
    IFC(m_pTesselationSink->SetTransform(m_Transform));
    
    IFC(ApplyBrush((const CGraphicsBrushBase*)pBrush));
    IFC(ApplyMask(NULL));
    
    IFC(pCoreGeometry->TesselateStroke(StrokeThickness, m_pTesselationSink)); 
    
Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRenderTargetBase::CreateLayer(
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
CRenderTargetBase::PushLayer(
	__in const CLayer* pLayer, 
	const RectF& ClippingRect,
	FLOAT Opacity, 
	__in_opt const CGraphicsGeometry* pClippingGeometry
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
CRenderTargetBase::PopLayer(
	)
{
    HRESULT hr = S_OK;

    //m_RenderTarget->PopLayer();

    return hr;
}

__checkReturn HRESULT
CRenderTargetBase::ApplyMask(
    __in_opt ITexture* pMask
    )   
{
    HRESULT hr = S_OK;
    CTextureAtlasView* pMaskView = NULL;
    Matrix3X2F maskTextureToBrushTransform;

    pMaskView = (CTextureAtlasView*)pMask;

    if (pMaskView == NULL)
    {
        if (m_pLastRenderedMaskAtlas != NULL)
        {
            pMaskView = (CTextureAtlasView*)m_pLastRenderedMaskAtlas->GetWhitePixelTexture();
        }
        else
        {
            pMaskView = m_pDefaultWhitePixelTexture;
        }
    }

    {
        ITexture* pMaskTexture = pMaskView->GetTexture();
        ITexture* pPreviousMask = (m_pLastRenderedMaskAtlas != NULL) ? m_pLastRenderedMaskAtlas->GetTexture() : NULL;

        if (pMaskTexture != pPreviousMask)
        {
            IFC(Flush())

            IFC(BindMask(pMaskTexture));
        }

        ReplaceObject(m_pLastRenderedMaskAtlas, (ITextureAtlasWithWhitePixel*)pMaskView->GetAtlas());

        {
            const RectU& viewBounds = pMaskView->GetRect();
            const FLOAT textureWidth = (FLOAT)pMaskTexture->GetWidth();
            const FLOAT textureHeight = (FLOAT)pMaskTexture->GetHeight();

            const FLOAT left = (viewBounds.left / textureWidth) + (0.5f / textureWidth);
            const FLOAT right = (viewBounds.right / textureWidth) - (0.5f / textureWidth);
            const FLOAT top = (viewBounds.top / textureHeight) + (0.5f / textureHeight);
            const FLOAT bottom = (viewBounds.bottom / textureHeight) - (0.5f / textureHeight);

            //
            // Scale from [0, 1] range to texture view UV range.
            //
            maskTextureToBrushTransform = Matrix3X2F::Scale(SizeF(right - left, bottom - top)) * Matrix3X2F::Translation(left, top);
        }
    }

    IFC(m_pTesselationSink->SetMaskTransform(&maskTextureToBrushTransform));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CRenderTargetBase::ApplyBrush(
    __in const CGraphicsBrushBase* pBrush
    )
{
    HRESULT hr = S_OK;
    CTextureAtlasView* pTextureView = NULL;
    Matrix3X2F textureToBrushTransform;
    
    pTextureView = (CTextureAtlasView*)pBrush->GetTexture();
    
    if (pTextureView == NULL)
    {
        if (m_pLastRenderedTextureAtlas != NULL)
        {
            pTextureView = (CTextureAtlasView*)m_pLastRenderedTextureAtlas->GetWhitePixelTexture();
        }
        else
        {
            pTextureView = m_pDefaultWhitePixelTexture;
        }
    }
    
    {
        ITexture* pTexture = pTextureView->GetTexture();

        ITexture* pPreviousTexture = (m_pLastRenderedTextureAtlas != NULL) ? m_pLastRenderedTextureAtlas->GetTexture() : NULL;
   
        //
        // The last rendered texture should only ever be NULL for the first draw in a frame.
        //
        if (pTexture != pPreviousTexture)
        {
            IFC(Flush());

            IFC(BindTexture(pTexture));
        }
        
        ReplaceObject(m_pLastRenderedTextureAtlas, (ITextureAtlasWithWhitePixel*)pTextureView->GetAtlas());
        
        {
            const RectU& viewBounds = pTextureView->GetRect();
            const FLOAT textureWidth = (FLOAT)pTexture->GetWidth();
            const FLOAT textureHeight = (FLOAT)pTexture->GetHeight();
            
            const FLOAT left = (viewBounds.left / textureWidth) + (0.5f / textureWidth);
            const FLOAT right = (viewBounds.right / textureWidth) - (0.5f / textureWidth);
            const FLOAT top = (viewBounds.top / textureHeight) + (0.5f / textureHeight);
            const FLOAT bottom = (viewBounds.bottom / textureHeight) - (0.5f / textureHeight);
            
            //
            // Scale from [0, 1] range to texture view UV range.
            //
            textureToBrushTransform = Matrix3X2F::Scale(SizeF(right - left, bottom - top)) * Matrix3X2F::Translation(left, top);
        }
    }
    
    IFC(ApplyBrushToTesselationSink(textureToBrushTransform, pBrush));    
    
Cleanup:
    return hr;
}

__checkReturn HRESULT
CRenderTargetBase::ApplyBrushToTesselationSink(
    const Matrix3X2F& textureToBrushTransform,
    __in const CGraphicsBrushBase* pBrush
    )
{
    HRESULT hr = S_OK;
    Matrix3X2F brushTransform;
    
    IFC(m_pTesselationSink->SetDiffuseColor(pBrush->GetDiffuseColor()));    

    //TODO: Optimize solid color brushes out.
    pBrush->GetFinalTransform(brushTransform);

    //
    // Invert the brush transform to normalize to a [0, 1] range.
    // 
    if(!brushTransform.Invert())
    {
        brushTransform = Matrix3X2F::Scale(0, 0);
    }
    
    {
        //
        // Combine the texture to brush transform and the brush transform.
        //
        Matrix3X2F finalTransform = brushTransform * textureToBrushTransform;
        
        IFC(m_pTesselationSink->SetBrushTransform(&finalTransform));    
    }
    
Cleanup:    
    return hr;
}

__checkReturn HRESULT
CRenderTargetBase::Flush(
    )
{
    HRESULT hr = S_OK;

    IFC(m_pTesselationSink->Flush());

Cleanup:    
    return hr;
}