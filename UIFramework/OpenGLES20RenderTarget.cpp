#include "OpenGLES20RenderTarget.h"
#include "OpenGLES20SolidColorBrush.h"
#include "OpenGLES20LinearGradientBrush.h"
#include "CoreRectangleGeometry.h"
#include "CoreRoundedRectangleGeometry.h"
#include "CorePathGraphicsGeometry.h"
#include "StaticTesselator.h"
#include <algorithm>

const char g_VertexShaderSource[] =
"attribute vec2 position;\n"
"attribute vec4 color;\n"
"attribute vec2 textureCoords;\n"
"\n"
"varying vec4 colorVarying;\n"
"varying vec2 textureCoordsVarying;\n"
"\n"
"uniform mat4 transform;\n"
"\n"
"void main()\n"
"{\n"
"gl_Position.xy = position;\n"
"gl_Position.z = 1.0;\n"
"gl_Position.w = 1.0;\n"
"gl_Position = transform * gl_Position;\n"
"\n"
"colorVarying = color;\n"
"\n"
"textureCoordsVarying = textureCoords;\n"
"}\n"
"\n";

const UINT32 g_VertexShaderSourceLength = ARRAYSIZE(g_VertexShaderSource);

const char g_PixelShaderSource[] =
"varying lowp vec4 colorVarying;\n"
"varying lowp vec2 textureCoordsVarying;\n"
"\n"
"uniform sampler2D brushTexture;\n"
"\n"
"void main()\n"
"{\n"
"gl_FragColor = colorVarying * texture2D(brushTexture, textureCoordsVarying);\n"
"}\n"
"\r\n";

const UINT32 g_PixelShaderSourceLength = ARRAYSIZE(g_PixelShaderSource);

COpenGLES20RenderTarget::COpenGLES20RenderTarget(
	) 
	: m_RenderBuffer(0)
	, m_FrameBuffer(0)
	, m_pContext(NULL)
	, m_Width(0)
	, m_Height(0)
    , m_Transform(Matrix3X2F::Identity())
    , m_ShaderProgram(0)
    , m_PositionAttribute(-1)
    , m_ColorAttribute(-1)
    , m_TextureCoordsAttribute(-1)
    , m_TransformUniform(-1)
    , m_BrushTextureUniform(-1)
    , m_pTesselationSink(NULL)
    , m_pTextureAtlasPool(NULL)
{
    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        m_pVertexBuffers[i] = NULL;
    }
}

COpenGLES20RenderTarget::~COpenGLES20RenderTarget(
	)
{
	IGNOREHR(ApplyContext());
    
    ReleaseObject(m_pTesselationSink);
    
    ReleaseObject(m_pTextureAtlasPool);
	
	if (m_FrameBuffer != 0)
	{
		glDeleteFramebuffers(1, &m_FrameBuffer);
	}

	if (m_RenderBuffer != 0)
	{
		glDeleteRenderbuffers(1, &m_RenderBuffer);
	}
    
    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        ReleaseObject(m_pVertexBuffers[i]);
    }
    
    if (m_ShaderProgram != 0)
    {
        glDeleteProgram(m_ShaderProgram);
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
    GLuint vertexBuffers[ARRAYSIZE(m_pVertexBuffers)] = { };
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

	m_RenderBuffer = RenderBuffer;
	m_FrameBuffer = FrameBuffer;

	m_pContext = pContext;

	IFC(ApplyContext());

	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_Width);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_Height);
    
    glGenBuffers(ARRAYSIZE(vertexBuffers), vertexBuffers);
    
    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        IFC(COpenGLES20VertexBuffer::Create(vertexBuffers[i], &m_pVertexBuffers[i]));     
        
        vertexBuffers[i] = 0;   
    }
    
    //TODO: Split out shader loader and cleanup.
    m_ShaderProgram = glCreateProgram();
    
    IFC(CreateShader(GL_VERTEX_SHADER, g_VertexShaderSource, &vertexShader));
    IFC(CreateShader(GL_FRAGMENT_SHADER, g_PixelShaderSource, &fragmentShader));
    
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);
    
    IFC(LinkProgram(m_ShaderProgram));
    
    {
        GLint validationStatus = 0;
        
        glValidateProgram(m_ShaderProgram);
        
        glGetProgramiv(m_ShaderProgram, GL_VALIDATE_STATUS, &validationStatus);
        
        IFCEXPECT(validationStatus != GL_FALSE);
    }
    
    m_PositionAttribute = glGetAttribLocation(m_ShaderProgram, "position");
    m_ColorAttribute = glGetAttribLocation(m_ShaderProgram, "color");
    m_TextureCoordsAttribute = glGetAttribLocation(m_ShaderProgram, "textureCoords");    
    
    m_TransformUniform = glGetUniformLocation(m_ShaderProgram, "transform");
    m_BrushTextureUniform = glGetUniformLocation(m_ShaderProgram, "brushTexture"); 
    
    IFC(COpenGLES20TesselationSink::Create(this, m_pVertexBuffers, ARRAYSIZE(m_pVertexBuffers), &m_pTesselationSink));
    
    {
        GLint maxTextureSize = 0;
        
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

        IFC(CTextureAtlasPool< TEXTURE_ATLAS_PADDING >::Create(maxTextureSize, maxTextureSize, this, &m_pTextureAtlasPool));
    }
    
Cleanup:
    for (UINT32 i = 0; i < ARRAYSIZE(m_pVertexBuffers); ++i)
    {
        if (vertexBuffers[i] != 0)
        {
            glDeleteBuffers(1, &vertexBuffers[i]);
        }
    }
    
    if (vertexShader != 0)
    {
        glDeleteShader(vertexShader);
    }
    
    if (fragmentShader != 0)
    {
        glDeleteShader(fragmentShader);
    }

    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::CreateShader(
    const GLenum ShaderType,
    __in_z const CHAR* pShaderSource,
    __out GLuint* pShader
    )
{
    HRESULT hr = S_OK;
    GLuint shader = 0;
    GLint compileStatus = 0;
#ifdef DEBUG
    GLchar* pLog = NULL;
#endif
    
    shader = glCreateShader(ShaderType);
    glShaderSource(shader, 1, &pShaderSource, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);        
    
#if defined(DEBUG)
    {
        GLint logLength;
        
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        
        if (logLength > 0)
        {
            pLog = new GLchar[logLength];
            IFCOOM(pLog);
            
            glGetShaderInfoLog(shader, logLength, &logLength, pLog);
            
            logging::DebugOut("%s", pLog);
        }
    }
#endif              
    
    IFCEXPECT(compileStatus != GL_FALSE);

    *pShader = shader;
    shader = 0;

Cleanup:
    if (shader != 0)
    {
        glDeleteShader(shader);
    }
    
#ifdef DEBUG
    delete [] pLog;
#endif    

    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::LinkProgram(
    GLuint Program
    )
{
    HRESULT hr = S_OK;
    GLint linkStatus = 0;
    CHAR* pLog = NULL;
    
    glLinkProgram(Program);
    
    glGetProgramiv(Program, GL_LINK_STATUS, &linkStatus);   
    
#if defined(DEBUG)
    {
        GLint logLength;
        
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &logLength);
        
        if (logLength > 0)
        {
            pLog = new GLchar[logLength];
            IFCOOM(pLog);
            
            glGetProgramInfoLog(Program, logLength, &logLength, pLog);
            
            logging::DebugOut("%s", pLog);
        }
    }
#endif        
    
    IFCEXPECT(linkStatus != GL_FALSE);
    
Cleanup:
#ifdef DEBUG
    delete [] pLog;    
#endif
    
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
    FLOAT projectionMatrix[16] = { };

    IFC(ApplyContext());

    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    
    glViewport(0, 0, m_Width, m_Height);
    
    //
    // Set up orthographic projection matrix.
    // 
    {
        FLOAT near = 0.001f;
        FLOAT far = 1000.0f;
        
        FLOAT left = 0.0f;
        FLOAT right = m_Width;
        FLOAT top = 0.0f;
        FLOAT bottom = m_Height;
        
        projectionMatrix[0] = 2.0f / (right - left);
        projectionMatrix[1] = 0.0f;
        projectionMatrix[2] = 0.0f;
        projectionMatrix[3] = 0.0f;
        
        projectionMatrix[4] = 0.0f;
        projectionMatrix[5] = 2.0f / (top - bottom);
        projectionMatrix[6] = 0.0f;
        projectionMatrix[7] = 0.0f;
        
        projectionMatrix[8] = 0.0f;
        projectionMatrix[9] = 0.0f;
        projectionMatrix[10] = 2.0f / (far - near);
        projectionMatrix[11] = 0.0f;
        
        projectionMatrix[12] = -(right + left) / (right - left);
        projectionMatrix[13] = -(top + bottom) / (top - bottom);
        projectionMatrix[14] = -(far + near) / (far - near);
        projectionMatrix[15] = 1.0f;
        
        glUniformMatrix4fv(m_TransformUniform, 1, 0, projectionMatrix);
    }
    
    //
    // Set up alpha blending.
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::EndRendering(
	)
{
    HRESULT hr = S_OK;

    IFC(m_pTesselationSink->Flush());

Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::SetTransform(
	const Matrix3X2F& Transform
	)
{
    HRESULT hr = S_OK;
    
    m_Transform = Transform;

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
    COpenGLES20SolidColorBrush* pSolidBrush = NULL;

     IFCPTR(ppBrush);

     IFC(COpenGLES20SolidColorBrush::Create(Color, &pSolidBrush));

     *ppBrush = pSolidBrush;
     pSolidBrush = NULL;

 Cleanup:
     ReleaseObject(pSolidBrush);

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
    ITexture* pGradientTexture = NULL;
    COpenGLES20LinearGradientBrush* pGradientBrush = NULL;
    GradientStop* pSortedGradientStops = NULL;
    BYTE* pColorBuffer = NULL;
    UINT32 colorBufferSize = 0;    
    PixelFormat::Value format = PixelFormat::Unknown;
    //TODO: Dynamically figure out how large this should be.
    UINT32 textureLength = 256 - (TEXTURE_ATLAS_PADDING * 2);
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
        FLOAT rotationAngle = atan2(direction.y, direction.x) * (180.0f / 3.1415927);
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
                texelColor = pSortedGradientStops[fromColorIndex].color;
            }
            else if(offset >= pSortedGradientStops[fromColorIndex + 1].position)
            {
                texelColor = pSortedGradientStops[fromColorIndex + 1].color;
            }
            else
            {
                FLOAT interpolateVal = (offset - pSortedGradientStops[fromColorIndex].position) / (pSortedGradientStops[fromColorIndex + 1].position -pSortedGradientStops[fromColorIndex].position);
                
                texelColor = ColorF::Interpolate(pSortedGradientStops[fromColorIndex].color, pSortedGradientStops[fromColorIndex + 1].color, interpolateVal);
            }
            
            switch(format)
            {
                case PixelFormat::B8G8R8A8:
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
        
    IFC(COpenGLES20LinearGradientBrush::Create(pGradientTexture, textureToBrushTransform, &pGradientBrush));        
    
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
    COpenGLES20Brush* pOpenGLESBrush = NULL;
    
    pOpenGLESBrush = (COpenGLES20Brush*)pBrush;    
    
    IFC(m_pTesselationSink->SetTransform(m_Transform));
    
    IFC(ApplyBrush(pOpenGLESBrush));
    
    //TODO: This should probably have stroke width...
    IFC(StaticTesselator::TesselateRectangleStroke(Size, 1.0f, m_pTesselationSink));
    
Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::FillRectangle(
	const RectF& Size,
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;
    COpenGLES20Brush* pOpenGLESBrush = NULL;
    
    pOpenGLESBrush = (COpenGLES20Brush*)pBrush;    
    
    IFC(m_pTesselationSink->SetTransform(m_Transform));
    
    IFC(ApplyBrush(pOpenGLESBrush));
    
    IFC(StaticTesselator::TesselateRectangle(Size, m_pTesselationSink));
    
Cleanup:
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
	__in const CGraphicsGeometry* pGeometry,
	__in const CGraphicsBrush* pBrush
	)
{
    HRESULT hr = S_OK;
    COpenGLES20Brush* pOpenGLESBrush = NULL;
    ICoreGeometry* pCoreGeometry = NULL;
    
    pOpenGLESBrush = (COpenGLES20Brush*)pBrush;

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
    
    IFC(ApplyBrush(pOpenGLESBrush));
    
    IFC(pCoreGeometry->TesselateFill(m_pTesselationSink)); 

Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20RenderTarget::DrawGeometry(
	__in const CGraphicsGeometry* pGeometry, 
	__in const CGraphicsBrush* pBrush, 
	FLOAT StrokeThickness
	)
{
    HRESULT hr = S_OK;
    COpenGLES20Brush* pOpenGLESBrush = NULL;
    ICoreGeometry* pCoreGeometry = NULL;
    
    pOpenGLESBrush = (COpenGLES20Brush*)pBrush;
    
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
    
    IFC(ApplyBrush(pOpenGLESBrush));
    
    IFC(pCoreGeometry->TesselateStroke(StrokeThickness, m_pTesselationSink)); 
    
Cleanup:
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
COpenGLES20RenderTarget::PopLayer(
	)
{
    HRESULT hr = S_OK;

    //m_RenderTarget->PopLayer();

    return hr;
}

__override __checkReturn HRESULT
COpenGLES20RenderTarget::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    COpenGLES20Texture* pOpenGLESTexture = NULL;
    
    IFC(CreateTexture(Width, Height, &pOpenGLESTexture));
    
    *ppTexture = pOpenGLESTexture;
    pOpenGLESTexture = NULL;
    
Cleanup:
    ReleaseObject(pOpenGLESTexture);
    
    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::CreateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out COpenGLES20Texture** ppTexture
    )
{
    HRESULT hr = S_OK;
    GLuint textureID = 0;
    COpenGLES20Texture* pOpenGLESTexture = NULL;
    
    IFC(ApplyContext());
    
    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
    
    IFC(COpenGLES20Texture::Create(textureID, Width, Height, PixelFormat::B8G8R8A8, &pOpenGLESTexture));
    
    textureID = 0;
    
    //TODO: Move this to atlas texture, handle gutters.
    {
        UINT32 stride = PixelFormat::GetLineSize(PixelFormat::B8G8R8A8, Width);
        UINT32 dataSize = stride * Height;
        BYTE* pData = new BYTE[dataSize];
        
        for(UINT32 i = 0; i < dataSize; ++i)
        {
            pData[i] = 0xFF;
        }
        
        pOpenGLESTexture->SetData(pData, dataSize, stride);
        
        delete [] pData;
    }
    
    *ppTexture = pOpenGLESTexture;
    pOpenGLESTexture = NULL;
    
Cleanup:
    if (textureID != 0)
    {
        glDeleteTextures(1, &textureID);
    }
    
    ReleaseObject(pOpenGLESTexture);
    
    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::ApplyBrush(
    __in const COpenGLES20Brush* pBrush
    )
{
    HRESULT hr = S_OK;
    ITexture* pBrushTexture = NULL;
    Matrix3X2F textureToBrushTransform;
    
    //TODO: Don't flush every brush change, use solid color pixel etc and check texture.
    IFC(Flush());
    
    pBrushTexture = pBrush->GetTexture();
    
    if (pBrushTexture != NULL)
    {
        CTextureAtlasView* pView = (CTextureAtlasView*)pBrushTexture;
        COpenGLES20Texture* pOpenGLESTexture = (COpenGLES20Texture*)pView->GetTexture();
            
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pOpenGLESTexture->GetTextureID());
        glUniform1i(m_BrushTextureUniform, 0);
        
        {
            const RectU& viewBounds = pView->GetRect();
            const FLOAT textureWidth = (FLOAT)pOpenGLESTexture->GetWidth();
            const FLOAT textureHeight = (FLOAT)pOpenGLESTexture->GetHeight();
            
            const FLOAT left = (viewBounds.left / textureWidth) + (0.5 / textureWidth);
            const FLOAT right = (viewBounds.right / textureWidth) - (0.5 / textureWidth);
            const FLOAT top = (viewBounds.top / textureHeight) + (0.5 / textureHeight);
            const FLOAT bottom = (viewBounds.bottom / textureHeight) - (0.5 / textureHeight);
            
            //
            // Scale from [0, 1] range to texture view UV range.
            //
            textureToBrushTransform = Matrix3X2F::Scale(SizeF(right - left, bottom - top), Point2F(left, top));
        }
    }
    else
    {
        //TODO: Apply solid color texture.
    }
    
    IFC(ApplyBrushToTesselationSink(textureToBrushTransform, pBrush));    
    
Cleanup:
    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::ApplyBrushToTesselationSink(
    const Matrix3X2F& textureToBrushTransform,
    __in const COpenGLES20Brush* pBrush
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
COpenGLES20RenderTarget::Flush(
    )
{
    HRESULT hr = S_OK;
    
    IFC(m_pTesselationSink->Flush());
    
Cleanup:    
    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::OnTesselatedGeometryBatch(
    __in COpenGLES20VertexBuffer* pVertexBuffer
    )
{
    HRESULT hr = S_OK;
    COpenGLES20VertexBuffer* pBuffer = NULL;
    UINT32 vertexCount = 0;
    
    vertexCount = pVertexBuffer->GetVertexCount();
    
    if (vertexCount > 0)
    {
        IFC(pVertexBuffer->Bind(GL_ARRAY_BUFFER));
        
        glUseProgram(m_ShaderProgram);
        
        if (m_PositionAttribute != -1)
        {
            glVertexAttribPointer(m_PositionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (const GLvoid*)RENDERVERTEX_POSITION_OFFSET);
            glEnableVertexAttribArray(m_PositionAttribute);
        }
        
        if (m_ColorAttribute != -1)
        {
            glVertexAttribPointer(m_ColorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (const GLvoid*)RENDERVERTEX_COLOR_OFFSET);
            glEnableVertexAttribArray(m_ColorAttribute);              
        }
        
        if (m_TextureCoordsAttribute != -1)
        {
            glVertexAttribPointer(m_TextureCoordsAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (const GLvoid*)RENDERVERTEX_TEXCOORDS_OFFSET);
            glEnableVertexAttribArray(m_TextureCoordsAttribute);
        }
        
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
    
Cleanup:
    return hr;
}