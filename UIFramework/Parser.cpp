#include "Parser.h"
#include "XMLReader.h"
#include "ParserCallback.h"

CParser::CParser() : m_Providers(NULL)
{
}

CParser::~CParser()
{
    ReleaseObject(m_Providers);
}

HRESULT CParser::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

Cleanup:
    return hr;
}

HRESULT CParser::LoadFromFile(const WCHAR* pPath, CObjectWithType** ppRootObject)
{
    HRESULT hr = S_OK;
    CXMLReader* pXMLReader = NULL;
    CParserCallback* pCallback = NULL;
    CParserCommandList* pCommandList = NULL;
    CParseContext* pContext = NULL;

    IFCPTR(pPath);
    IFCPTR(ppRootObject);

    IFC(CParseContext::Create(m_Providers, &pContext));

    IFC(CParserCallback::Create(pContext, &pCallback));

    IFC(CParserCommandList::Create(m_Providers, &pCommandList));

    IFC(pContext->PushCommandList(pCommandList));

    IFC(CreateXMLReader(&pXMLReader));

    IFC(pXMLReader->LoadFromFile(pPath, pCallback));

    IFC(pCommandList->Execute(NULL, ppRootObject));

Cleanup:
    ReleaseObject(pCallback);
    ReleaseObject(pXMLReader);
    ReleaseObject(pCommandList);
    ReleaseObject(pContext);

    return hr;
}

HRESULT CParser::LoadFromString(const WCHAR* pMarkup, CObjectWithType** ppRootObject)
{
    HRESULT hr = S_OK;
    CXMLReader* pXMLReader = NULL;
    CParserCallback* pCallback = NULL;
    CParserCommandList* pCommandList = NULL;
    CParseContext* pContext = NULL;

    IFCPTR(pMarkup);
    IFCPTR(ppRootObject);

    IFC(CParseContext::Create(m_Providers, &pContext));

    IFC(CParserCallback::Create(pContext, &pCallback));

    IFC(CParserCommandList::Create(m_Providers, &pCommandList));

    IFC(pContext->PushCommandList(pCommandList));

    IFC(CreateXMLReader(&pXMLReader));

    IFC(pXMLReader->LoadFromString(pMarkup, pCallback));

    IFC(pCommandList->Execute(NULL, ppRootObject));

Cleanup:
    ReleaseObject(pCallback);
    ReleaseObject(pXMLReader);
    ReleaseObject(pCommandList);
    ReleaseObject(pContext);

    return hr;
}

//
// CParser
//
extern "C" __declspec(dllexport)
void CParser_AddRef(CParser* pParser)
{
    pParser->AddRef();
}

extern "C" __declspec(dllexport)
void CParser_Release(CParser* pParser)
{
    pParser->Release();
}

extern "C" __declspec(dllexport)
HRESULT CParser_LoadFromFile(CParser* pParser, const WCHAR* pPath, CObjectWithType** ppRootObject)
{
    return pParser->LoadFromFile(pPath, ppRootObject);
}

extern "C" __declspec(dllexport)
HRESULT CParser_LoadFromString(CParser* pParser, const WCHAR* pMarkup, CObjectWithType** ppRootObject)
{
    return pParser->LoadFromString(pMarkup, ppRootObject);
}