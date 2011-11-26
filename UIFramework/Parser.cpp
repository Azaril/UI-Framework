#include "Parser.h"
#include "XMLReader.h"
#include "ParserCallback.h"

CParser::CParser(
	) 
	: m_Providers(NULL)
{
}

CParser::~CParser(
	)
{
    ReleaseObject(m_Providers);
}

__checkReturn HRESULT 
CParser::Initialize(
	__in CProviders* pProviders
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    m_Providers = pProviders;
    AddRefObject(m_Providers);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CParser::LoadFromFile(
	__in_z const WCHAR* pPath, 
	__deref_out CObjectWithType** ppRootObject
	)
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

__checkReturn HRESULT 
CParser::LoadFromString(
	__in_z const WCHAR* pMarkup,
	__deref_out CObjectWithType** ppRootObject
	)
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

__checkReturn HRESULT 
CParser::LoadFromStream(
    __in IReadStream* pStream,
    __deref_out CObjectWithType** ppRootObject
    )
{
    HRESULT hr = S_OK;
    CXMLReader* pXMLReader = NULL;
    CParserCallback* pCallback = NULL;
    CParserCommandList* pCommandList = NULL;
    CParseContext* pContext = NULL;
    
    IFCPTR(pStream);
    IFCPTR(ppRootObject);
    
    IFC(CParseContext::Create(m_Providers, &pContext));
    
    IFC(CParserCallback::Create(pContext, &pCallback));
    
    IFC(CParserCommandList::Create(m_Providers, &pCommandList));
    
    IFC(pContext->PushCommandList(pCommandList));
    
    IFC(CreateXMLReader(&pXMLReader));
    
    IFC(pXMLReader->LoadFromStream(pStream, pCallback));
    
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
extern "C" UIFRAMEWORK_API
void 
CParser_AddRef(
	__in CParser* pParser
	)
{
    pParser->AddRef();
}

extern "C" UIFRAMEWORK_API
void 
CParser_Release(
	__in CParser* pParser
	)
{
    pParser->Release();
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT 
CParser_LoadFromFile(
	__in CParser* pParser, 
	__in_z const WCHAR* pPath, 
	__deref_out CObjectWithType** ppRootObject
	)
{
    return pParser->LoadFromFile(pPath, ppRootObject);
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT 
CParser_LoadFromString(
	__in CParser* pParser, 
	__in_z const WCHAR* pMarkup, 
	__deref_out CObjectWithType** ppRootObject
	)
{
    return pParser->LoadFromString(pMarkup, ppRootObject);
}