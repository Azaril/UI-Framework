#include "OpenGLES20RenderTarget.h"
#include "SolidColorBrushBase.h"
#include "LinearGradientBrushBase.h"
#include "CoreRectangleGeometry.h"
#include "CoreRoundedRectangleGeometry.h"
#include "CorePathGraphicsGeometry.h"
#include "StaticTesselator.h"
#include "TextureAtlasPool.h"
#include "TextureAtlasWithWhitePixel.h"
#include <algorithm>

const char g_VertexShaderSource[] =
"attribute vec2 position;\n"
"attribute vec4 color;\n"
"attribute vec2 textureCoords;\n"
"attribute vec2 maskCoords;\n"
"\n"
"varying vec4 colorVarying;\n"
"varying vec2 textureCoordsVarying;\n"
"varying vec2 maskCoordsVarying;\n"
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
"maskCoordsVarying = maskCoords;\n"
"}\n"
"\n";

const UINT32 g_VertexShaderSourceLength = ARRAYSIZE(g_VertexShaderSource);

const char g_PixelShaderSource[] =
"varying lowp vec4 colorVarying;\n"
"varying lowp vec2 textureCoordsVarying;\n"
"varying lowp vec2 maskCoordsVarying;\n"
"\n"
"uniform sampler2D brushTexture;\n"
"uniform sampler2D maskTexture;\n"
"\n"
"void main()\n"
"{\n"
"gl_FragColor = colorVarying * texture2D(brushTexture, textureCoordsVarying) * texture2D(maskTexture, maskCoordsVarying);\n"
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
    , m_ShaderProgram(0)
    , m_PositionAttribute(-1)
    , m_ColorAttribute(-1)
    , m_TextureCoordsAttribute(-1)
    , m_MaskCoordsAttribute(-1)
    , m_TransformUniform(-1)
    , m_BrushTextureUniform(-1)
    , m_MaskTextureUniform(-1)
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
	__in_opt COpenGLES20Context* pContext,
    __in CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >* pTextureAtlasPool
	)
{
    HRESULT hr = S_OK;
    GLuint vertexBuffers[ARRAYSIZE(m_pVertexBuffers)] = { };
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    CGeometryTesselationSink* pTesselationSink = NULL;
    CTextureAtlasWithWhitePixel< 1 >* pFirstTextureAtlas = NULL;

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
    m_MaskCoordsAttribute = glGetAttribLocation(m_ShaderProgram, "maskCoords");
    
    m_TransformUniform = glGetUniformLocation(m_ShaderProgram, "transform");
    m_BrushTextureUniform = glGetUniformLocation(m_ShaderProgram, "brushTexture"); 
    m_MaskTextureUniform = glGetUniformLocation(m_ShaderProgram, "maskTexture");
    
    IFC(CGeometryTesselationSink::Create(this, (IVertexBuffer**)m_pVertexBuffers, ARRAYSIZE(m_pVertexBuffers), &pTesselationSink));
    
    IFC(pTextureAtlasPool->GetOrCreateFirstTextureAtlas(&pFirstTextureAtlas));
    
    IFC(CRenderTargetBase::Initialize(pTesselationSink, pTextureAtlasPool, (CTextureAtlasView*)pFirstTextureAtlas->GetWhitePixelTexture()));
    
Cleanup:
    ReleaseObject(pTesselationSink);
    ReleaseObject(pTextureAtlasPool);
    ReleaseObject(pFirstTextureAtlas);
    
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

    IFC(CRenderTargetBase::BeginRendering());

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

    IFC(CRenderTargetBase::EndRendering());

Cleanup:
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
COpenGLES20RenderTarget::Flush(
    )
{
    HRESULT hr = S_OK;
    
    IFC(CRenderTargetBase::Flush());
    
Cleanup:    
    return hr;
}

__checkReturn HRESULT
COpenGLES20RenderTarget::OnTesselatedGeometryBatch(
    __in IVertexBuffer* pVertexBuffer
    )
{
    HRESULT hr = S_OK;
    COpenGLES20VertexBuffer* pBuffer = NULL;
    UINT32 vertexCount = 0;
    
    pBuffer = (COpenGLES20VertexBuffer*)pVertexBuffer;
    
    vertexCount = pVertexBuffer->GetVertexCount();
    
    if (vertexCount > 0)
    {
        IFC(pBuffer->Bind(GL_ARRAY_BUFFER));
        
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

__override __checkReturn HRESULT
COpenGLES20RenderTarget::BindTexture(
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;
    COpenGLES20Texture* pOpenGLESTexture = NULL;
    
    pOpenGLESTexture = (COpenGLES20Texture*)pTexture;
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pOpenGLESTexture->GetTextureID());
    glUniform1i(m_BrushTextureUniform, 0);
    
    return hr;
}

__override __checkReturn HRESULT
COpenGLES20RenderTarget::BindMask(
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;
    COpenGLES20Texture* pOpenGLESTexture = NULL;
    
    pOpenGLESTexture = (COpenGLES20Texture*)pTexture;
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pOpenGLESTexture->GetTextureID());
    glUniform1i(m_MaskTextureUniform, 0);
    
    return hr;
}