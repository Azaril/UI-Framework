#include "UIFrameworkBridge.h"
#include "BasicTypeConverter.h"
#include "DynamicClassResolver.h"
#include "Parser.h"
#include "UIHost.h"
#include "FileResourceProvider.h"
#include "BundleFileResourceProvider.h"
#include "CompositeResourceProvider.h"
#include "OpenGLES20GraphicsDevice.h"
#include "OpenGLES20RenderTarget.h"

UIFrameworkBridge::UIFrameworkBridge(
    )
    : m_pClassResolver(NULL)
    , m_pTypeConverter(NULL)
    , m_pProviders(NULL)
    , m_pParser(NULL)
    , m_pGraphicsDevice(NULL)
    , m_pRenderTarget(NULL)
    , m_pUIHost(NULL)
    , m_pFileResourceProvider(NULL)
    , m_pBundleResourceProvider(NULL)
    , m_pCompositeResourceProvider(NULL)
{
}

UIFrameworkBridge::~UIFrameworkBridge(
    )
{
    ReleaseObject(m_pFileResourceProvider);
    ReleaseObject(m_pBundleResourceProvider);
    ReleaseObject(m_pCompositeResourceProvider);
    ReleaseObject(m_pClassResolver);
    ReleaseObject(m_pTypeConverter);
    ReleaseObject(m_pProviders);
    ReleaseObject(m_pParser);
    ReleaseObject(m_pRenderTarget);
    ReleaseObject(m_pGraphicsDevice);
    ReleaseObject(m_pUIHost);
}

bool
UIFrameworkBridge::Initialize(
    COpenGLES20Context* pContext,
    COpenGLES20RenderBufferStorageAllocator* pAllocator                              
    )
{
    HRESULT hr = S_OK;
    
    IFC(CDynamicClassResolver::Create(&m_pClassResolver));
    
    IFC(CreateBasicTypeConverter(&m_pTypeConverter));
    
    IFC(CFileResourceProvider::Create(&m_pFileResourceProvider));
    
    IFC(CBundleFileResourceProvider::Create(&m_pBundleResourceProvider));
    
    IFC(CCompositeResourceProvider::Create(&m_pCompositeResourceProvider));
    
    IFC(m_pCompositeResourceProvider->AddResourceProvider(m_pBundleResourceProvider));
    IFC(m_pCompositeResourceProvider->AddResourceProvider(m_pFileResourceProvider));
    
    IFC(CProviders::Create(m_pClassResolver, m_pTypeConverter, m_pCompositeResourceProvider, &m_pProviders));
    
    IFC(CParser::Create(m_pProviders, &m_pParser));
    
    IFC(COpenGLES20GraphicsDevice::Create(&m_pGraphicsDevice));
    
    IFC(m_pGraphicsDevice->CreateRenderTarget(pAllocator, pContext, &m_pRenderTarget));
    
    IFC(CUIHost::Create(m_pGraphicsDevice, m_pRenderTarget, m_pProviders, &m_pUIHost));
    
Cleanup:
    return SUCCEEDED(hr);
}

bool
UIFrameworkBridge::Render(
    )
{
    HRESULT hr = S_OK;

    if (m_pUIHost != NULL)
    {
        IFC(m_pUIHost->Render());
    }
    
Cleanup:
    return SUCCEEDED(hr);
}

unsigned int
UIFrameworkBridge::GetRenderBuffer(
    )
{
    return m_pRenderTarget->GetRenderBuffer();
}

bool
UIFrameworkBridge::LoadContent(
    const wchar_t* pContent
)
{
    HRESULT hr = S_OK;
    CObjectWithType* pParsedRootObject = NULL;
    CRootUIElement* pRootElement = NULL;
    CUIElement* pParsedRootElement = NULL;
    
    IFC(m_pParser->LoadFromString(pContent, &pParsedRootObject));
    
    IFC(m_pUIHost->GetRootElement(&pRootElement));
    
    IFC(CastType(pParsedRootObject, &pParsedRootElement));
    
    IFC(pRootElement->SetChild(pParsedRootElement));
    
Cleanup:
    ReleaseObject(pRootElement);
    ReleaseObject(pParsedRootObject);
    
    return SUCCEEDED(hr);
}