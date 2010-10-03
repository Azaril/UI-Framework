#include "Providers.h"
#include "ErrorChecking.h"
#include "BindingManager.h"

CProviders::CProviders() : m_ClassResolver(NULL),
                           m_TypeConverter(NULL),
                           m_BindingManager(NULL)
{
}

CProviders::~CProviders()
{
    ReleaseObject(m_ClassResolver);
    ReleaseObject(m_TypeConverter);
    ReleaseObject(m_BindingManager);
}

HRESULT CProviders::Initialize( CClassResolver* pClassResolver, CTypeConverter* pTypeConverter )
{
    HRESULT hr = S_OK;

    IFCPTR(pClassResolver);
    IFCPTR(pTypeConverter);

    m_ClassResolver = pClassResolver;
    AddRefObject(m_ClassResolver);

    m_TypeConverter = pTypeConverter;
    AddRefObject(m_TypeConverter);

    IFC(CBindingManager::Create(&m_BindingManager));

Cleanup:
    return hr;
}

CClassResolver* CProviders::GetClassResolver()
{
    return m_ClassResolver;
}

CTypeConverter* CProviders::GetTypeConverter()
{
    return m_TypeConverter;
}

CBindingManager* CProviders::GetBindingManager()
{
    return m_BindingManager;
}