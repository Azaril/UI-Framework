#include "OpenGLES20GraphicsDevice.h"
#include "OpenGLES20RenderTarget.h"
#include "CoreGeometryProvider.h"
#include "CGImagingProvider.h"
#include "FreetypeTextProvider.h"

COpenGLES20GraphicsDevice::COpenGLES20GraphicsDevice(
    )
    : m_pTextProvider(NULL)
    , m_pImagingProvider(NULL)
    , m_pGeometryProvider(NULL)
    , m_pContext(NULL)
    , m_pTextureAtlasPool(NULL)
{
}

COpenGLES20GraphicsDevice::~COpenGLES20GraphicsDevice(
    )
{
	ReleaseObject(m_pTextProvider);
	ReleaseObject(m_pImagingProvider);
    ReleaseObject(m_pGeometryProvider);
    ReleaseObject(m_pContext);
}

__checkReturn HRESULT
COpenGLES20GraphicsDevice::Initialize(
    __in_opt COpenGLES20Context* pContext
    )
{
    HRESULT hr = S_OK;
    
    SetObject(m_pContext, pContext);
    
    if (m_pContext != NULL)
    {
        IFC(m_pContext->Apply());
    }
    
    {
        GLint maxTextureSize = 0;
        
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        
        IFC(CTextureAtlasPool< CTextureAtlasWithWhitePixel< 1 > >::Create(maxTextureSize, maxTextureSize, this, &m_pTextureAtlasPool));
    }
    
    IFC(CreateTextProvider(&m_pTextProvider));
    
    IFC(CreateImagingProvider(&m_pImagingProvider));
    
    IFC(CreateGeometryProvider(&m_pGeometryProvider));    
    
Cleanup:
    return hr;
}

__checkReturn HRESULT
COpenGLES20GraphicsDevice::CreateRenderTarget(
    __in COpenGLES20RenderBufferStorageAllocator* pAllocator,
    __deref_out COpenGLES20RenderTarget** ppRenderTarget
	)
{
	HRESULT hr = S_OK;
	GLuint RenderBuffer = 0;
	GLuint FrameBuffer = 0;
	COpenGLES20RenderTarget* pRenderTarget = NULL;

	if (m_pContext != NULL)
	{
		IFCEXPECT(m_pContext->Apply());
	}

	glGenRenderbuffers(1, &RenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);

	IFCEXPECT(pAllocator->AllocateStorage(GL_RENDERBUFFER));

	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RenderBuffer);

	IFC(COpenGLES20RenderTarget::Create(RenderBuffer, FrameBuffer, m_pContext, m_pTextureAtlasPool, &pRenderTarget));

	RenderBuffer = 0;
	FrameBuffer = 0;

	*ppRenderTarget = pRenderTarget;
	pRenderTarget = NULL;
	
Cleanup:
	if (FrameBuffer != 0)
	{
		glDeleteFramebuffers(1, &FrameBuffer);
	}

	if (RenderBuffer != 0)
	{
		glDeleteRenderbuffers(1, &RenderBuffer);
	}

	ReleaseObject(pRenderTarget);

	return hr;	
}

__override __checkReturn HRESULT 
COpenGLES20GraphicsDevice::GetTextProvider(
	__deref_out CTextProvider** ppTextProvider
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppTextProvider);

    *ppTextProvider = m_pTextProvider;
    AddRefObject(m_pTextProvider);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
COpenGLES20GraphicsDevice::GetImagingProvider(
	CImagingProvider** ppImagingProvider
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppImagingProvider);

    *ppImagingProvider = m_pImagingProvider;
    AddRefObject(m_pImagingProvider);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
COpenGLES20GraphicsDevice::GetGeometryProvider(
	CGeometryProvider** ppGeometryProvider
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppGeometryProvider);

    *ppGeometryProvider = m_pGeometryProvider;
    AddRefObject(m_pGeometryProvider);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
COpenGLES20GraphicsDevice::CreateTextProvider(
	__deref_out CTextProvider** ppTextProvider
	)
{
    HRESULT hr = S_OK;
    CFreetypeTextProvider* pFreetypeTextProvider = NULL;

    IFCPTR(ppTextProvider);

    if(SUCCEEDED(CFreetypeTextProvider::Create(m_pTextureAtlasPool, &pFreetypeTextProvider)))
    {
        *ppTextProvider = pFreetypeTextProvider;
        pFreetypeTextProvider = NULL;
        goto Cleanup;
    }

    IFC(E_FAIL);

Cleanup:
    ReleaseObject(pFreetypeTextProvider);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20GraphicsDevice::CreateImagingProvider(
	__deref_out CImagingProvider** ppImagingProvider
	)
{
    HRESULT hr = S_OK;
    CCGImagingProvider* pCGImagingProvider = NULL;

    if(SUCCEEDED(CCGImagingProvider::Create(&pCGImagingProvider)))
    {
        *ppImagingProvider = pCGImagingProvider;
        pCGImagingProvider = NULL;
        goto Cleanup;
    }

    IFC(E_FAIL);

Cleanup:
    ReleaseObject(pCGImagingProvider);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20GraphicsDevice::CreateGeometryProvider(
	__deref_out CGeometryProvider** ppGeometryProvider
	)
{
    HRESULT hr = S_OK;
    CCoreGeometryProvider* pCoreGeometryProvider = NULL;

    IFCPTR(ppGeometryProvider);

    if(SUCCEEDED(CCoreGeometryProvider::Create(&pCoreGeometryProvider)))
    {
        *ppGeometryProvider = pCoreGeometryProvider;
        pCoreGeometryProvider = NULL;
        goto Cleanup;
    }

    IFC(E_FAIL);

Cleanup:
    ReleaseObject(pCoreGeometryProvider);

    return hr;
}

__override __checkReturn HRESULT
COpenGLES20GraphicsDevice::AllocateTexture(
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

__override __checkReturn HRESULT
COpenGLES20GraphicsDevice::AllocateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out IBatchUpdateTexture** ppTexture
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
COpenGLES20GraphicsDevice::CreateTexture(
    UINT32 Width,
    UINT32 Height,
    __deref_out COpenGLES20Texture** ppTexture
    )
{
    HRESULT hr = S_OK;
    GLuint textureID = 0;
    COpenGLES20Texture* pOpenGLESTexture = NULL;
    
    if (m_pContext != NULL)
    {
        IFC(m_pContext->Apply());
    }
    
    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
    
    IFC(COpenGLES20Texture::Create(textureID, Width, Height, PixelFormat::R8G8B8A8, &pOpenGLESTexture));
    
    textureID = 0;
    
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