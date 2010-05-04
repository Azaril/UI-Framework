#include "ParseContext.h"

CParseContext::CParseContext() : m_ClassResolver(NULL),
                                 m_TypeConverter(NULL)
{
}

CParseContext::~CParseContext()
{
    ReleaseObject(m_ClassResolver);
    ReleaseObject(m_TypeConverter);
}

HRESULT CParseContext::Initialize(CClassResolver* pClassResolver, CTypeConverter* pTypeConverter)
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

CClassResolver* CParseContext::GetClassResolver()
{
    return m_ClassResolver;
}

CTypeConverter* CParseContext::GetTypeConverter()
{
    return m_TypeConverter;
}