#include "OpenGLES20GraphicsDevice.h"
#include "OpenGLES20RenderTarget.h"
#include "CoreGeometryProvider.h"

COpenGLES20GraphicsDevice::COpenGLES20GraphicsDevice(
    )
    : m_pTextProvider(NULL)
    , m_pImagingProvider(NULL)
    , m_pGeometryProvider(NULL)
{
}

COpenGLES20GraphicsDevice::~COpenGLES20GraphicsDevice(
    )
{
	ReleaseObject(m_pTextProvider);
	ReleaseObject(m_pImagingProvider);
    ReleaseObject(m_pGeometryProvider);
}

__checkReturn HRESULT
COpenGLES20GraphicsDevice::Initialize(
    )
{
    HRESULT hr = S_OK;

    IFC(CreateTextProvider(&m_pTextProvider));

    IFC(CreateImagingProvider(&m_pImagingProvider));

    IFC(CreateGeometryProvider(&m_pGeometryProvider));
    
Cleanup:
    return hr;
}

__checkReturn HRESULT
COpenGLES20GraphicsDevice::CreateRenderTarget(
	__in COpenGLES20RenderBufferStorageAllocator* pAllocator,
	__in_opt COpenGLES20Context* pContext,
    __deref_out COpenGLES20RenderTarget** ppRenderTarget
	)
{
	HRESULT hr = S_OK;
	GLuint RenderBuffer = 0;
	GLuint FrameBuffer = 0;
	COpenGLES20RenderTarget* pRenderTarget = NULL;

	if (pContext != NULL)
	{
		IFCEXPECT(pContext->Apply());
	}

	glGenRenderbuffers(1, &RenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);

	IFCEXPECT(pAllocator->AllocateStorage(GL_RENDERBUFFER));

	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RenderBuffer);

	IFC(COpenGLES20RenderTarget::Create(RenderBuffer, FrameBuffer, pContext, &pRenderTarget));

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
    // CDirectWriteTextProvider* pDirectWriteTextProvider = NULL;

    IFCPTR(ppTextProvider);

    // if(SUCCEEDED(CDirectWriteTextProvider::Create(&pDirectWriteTextProvider)))
    // {
    //     *ppTextProvider = pDirectWriteTextProvider;
    //     pDirectWriteTextProvider = NULL;
    //     goto Cleanup;
    // }

    //IFC(E_FAIL);

Cleanup:
    // ReleaseObject(pDirectWriteTextProvider);

    return hr;
}

__checkReturn HRESULT 
COpenGLES20GraphicsDevice::CreateImagingProvider(
	__deref_out CImagingProvider** ppImagingProvider
	)
{
    HRESULT hr = S_OK;
    // CWICImagingProvider* pWICImagingProvider = NULL;

    IFCPTR(ppImagingProvider);

    // if(SUCCEEDED(CWICImagingProvider::Create(&pWICImagingProvider)))
    // {
    //     *ppImagingProvider = pWICImagingProvider;
    //     pWICImagingProvider = NULL;
    //     goto Cleanup;
    // }

    //IFC(E_FAIL);

Cleanup:
    // ReleaseObject(pWICImagingProvider);

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