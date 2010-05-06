#include "Providers.h"
#include "ErrorChecking.h"

CProviders::CProviders() : m_ClassResolver(NULL),
               m_TypeConverter(NULL)
{
}

CProviders::~CProviders()
{
    ReleaseObject(m_ClassResolver);
    ReleaseObject(m_TypeConverter);
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