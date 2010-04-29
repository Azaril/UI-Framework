#include "Parser.h"
#include "XMLReader.h"
#include "ParserCallback.h"

CParser::CParser() : m_ClassResolver(NULL),
                     m_TypeConverter(NULL)
{
}

CParser::~CParser()
{
    ReleaseObject(m_ClassResolver);
    ReleaseObject(m_TypeConverter);
}

HRESULT CParser::Initialize(CClassResolver* pResolver, CTypeConverter* pTypeConverter)
{
    HRESULT hr = S_OK;

    IFCPTR(pResolver);

    m_ClassResolver = pResolver;
    AddRefObject(m_ClassResolver);

    m_TypeConverter = pTypeConverter;
    AddRefObject(m_TypeConverter);

Cleanup:
    return hr;
}

HRESULT CParser::LoadFromFile(const WCHAR* pPath, CUIElement** pRootElement)
{
    HRESULT hr = S_OK;
    CXMLReader* pXMLReader = NULL;
    CParserCallback* pCallback = NULL;

    IFCPTR(pPath);
    IFCPTR(pRootElement);

    IFC(CParserCallback::Create(m_ClassResolver, m_TypeConverter, &pCallback));

    IFC(CreateXMLReader(&pXMLReader));

    IFC(pXMLReader->LoadFromFile(pPath, pCallback));

    IFC(pCallback->GetRootElement(pRootElement));

Cleanup:
    ReleaseObject(pCallback);
    ReleaseObject(pXMLReader);

    return hr;
}