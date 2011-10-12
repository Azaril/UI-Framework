#pragma once

class CDynamicClassResolver;
class CTypeConverter;
class CProviders;
class CParser;
class CUIHost;
class COpenGLES20GraphicsDevice;
class COpenGLES20RenderBufferStorageAllocator;
class COpenGLES20RenderTarget;
class COpenGLES20Context;
class CFileResourceProvider;
class CBundleFileResourceProvider;
class CCompositeResourceProvider;

class UIFrameworkBridge
{
public:
    UIFrameworkBridge(
    );
    
    virtual ~UIFrameworkBridge(
    );
    
    bool Initialize(
        COpenGLES20Context* pContext,
        COpenGLES20RenderBufferStorageAllocator* pAllocator
        );
    
    bool Render(
        );
    
    unsigned int GetRenderBuffer(
        );
    
    bool LoadContent(
        const wchar_t* pText
        );
    
    CDynamicClassResolver* m_pClassResolver;
    CTypeConverter* m_pTypeConverter;
    CProviders* m_pProviders;
    CParser* m_pParser;
    CUIHost* m_pUIHost;
    CFileResourceProvider* m_pFileResourceProvider;
    CBundleFileResourceProvider* m_pBundleResourceProvider;
    CCompositeResourceProvider* m_pCompositeResourceProvider;
    COpenGLES20GraphicsDevice* m_pGraphicsDevice;
    COpenGLES20RenderTarget* m_pRenderTarget;
};