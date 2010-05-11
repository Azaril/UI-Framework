#include "ParseContext.h"

CParseContext::CParseContext() : m_Providers(NULL),
                                 m_ClassResolver(NULL),
                                 m_TypeConverter(NULL)
{
}

CParseContext::~CParseContext()
{
    ReleaseObject(m_ClassResolver);
    ReleaseObject(m_TypeConverter);
    ReleaseObject(m_Providers);

    for(std::vector< CParserCommandList* >::iterator It = m_CommandLists.begin(); It != m_CommandLists.end(); ++It)
    {
        (*It)->Release();
    }

    m_CommandLists.clear();
}

HRESULT CParseContext::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

    m_ClassResolver = m_Providers->GetClassResolver();
    IFCPTR(m_ClassResolver);
    AddRefObject(m_ClassResolver);

    m_TypeConverter = m_Providers->GetTypeConverter();
    IFCPTR(m_TypeConverter);
    AddRefObject(m_TypeConverter);

Cleanup:
    return hr;
}

CProviders* CParseContext::GetProviders()
{
    return m_Providers;
}

CClassResolver* CParseContext::GetClassResolver()
{
    return m_ClassResolver;
}

CTypeConverter* CParseContext::GetTypeConverter()
{
    return m_TypeConverter;
}

HRESULT CParseContext::GetCommandList(CParserCommandList** ppCommandList)
{
    HRESULT hr = S_OK;
    CParserCommandList* pList = NULL;

    IFCPTR(ppCommandList);

    IFCEXPECT(!m_CommandLists.empty());

    pList = m_CommandLists.back();

    *ppCommandList = pList;
    AddRefObject(pList);

Cleanup:
    return hr;
}

HRESULT CParseContext::PushCommandList(CParserCommandList* pCommandList)
{
    HRESULT hr = S_OK;

    IFCPTR(pCommandList);

    m_CommandLists.push_back(pCommandList);
    AddRefObject(pCommandList);

Cleanup:
    return hr;
}

HRESULT CParseContext::PopCommandList()
{
    HRESULT hr = S_OK;
    CParserCommandList* pList = NULL;

    IFCEXPECT(!m_CommandLists.empty());

    pList = m_CommandLists.back();
    ReleaseObject(pList);

    m_CommandLists.pop_back();

Cleanup:
    return hr;
}

HRESULT CParseContext::AddCommand(CParserCommand* pCommand)
{
    HRESULT hr = S_OK;
    CParserCommandList* pCommandList = NULL;

    IFCPTR(pCommand);

    IFC(GetCommandList(&pCommandList));

    IFC(pCommandList->AddCommand(pCommand));

Cleanup:
    ReleaseObject(pCommandList);

    return hr;
}