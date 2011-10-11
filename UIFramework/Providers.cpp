#include "Providers.h"
#include "ErrorChecking.h"
#include "BindingManager.h"
#include "ResourceProvider.h"

CProviders::CProviders(
    ) 
    : m_pClassResolver(NULL)
    , m_pTypeConverter(NULL)
    , m_pBindingManager(NULL)
    , m_pResourceProvider(NULL)
{
}

CProviders::~CProviders()
{
    ReleaseObject(m_pClassResolver);
    ReleaseObject(m_pTypeConverter);
    ReleaseObject(m_pBindingManager);
    ReleaseObject(m_pResourceProvider);
}

__checkReturn HRESULT 
CProviders::Initialize( 
    __in CClassResolver* pClassResolver, 
    __in CTypeConverter* pTypeConverter,
    __in IResourceProvider* pResourceProvider
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pClassResolver);
    IFCPTR(pTypeConverter);

    SetObject(m_pClassResolver, pClassResolver);
    SetObject(m_pTypeConverter, pTypeConverter);
    SetObject(m_pResourceProvider, pResourceProvider);

    IFC(CBindingManager::Create(&m_pBindingManager));

Cleanup:
    return hr;
}

__out CClassResolver* 
CProviders::GetClassResolver(
    )
{
    return m_pClassResolver;
}

__out CTypeConverter*
CProviders::GetTypeConverter(
    )
{
    return m_pTypeConverter;
}

__out CBindingManager* 
CProviders::GetBindingManager(
    )
{
    return m_pBindingManager;
}

__out IResourceProvider*
CProviders::GetResourceProvider( 
    )
{
    return m_pResourceProvider;
}