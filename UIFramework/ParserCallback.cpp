#include "ParserCallback.h"
#include "BasicTypes.h"
#include "ParserUtilities.h"

CParserCallback::CParserCallback() : m_ChildNode(NULL),
                                     m_ParseContext(NULL)
{
}

CParserCallback::~CParserCallback()
{
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_ParseContext);
}

HRESULT CParserCallback::Initialize(CClassResolver* pClassResolver, CTypeConverter* pTypeConverter)
{
    HRESULT hr = S_OK;

    IFCPTR(pClassResolver);

    IFC(CParseContext::Create(pClassResolver, pTypeConverter, &m_ParseContext));

Cleanup:
    return hr;
}

HRESULT CParserCallback::GetRootObject(CObjectWithType** ppRootObject)
{
    HRESULT hr = S_OK;
    CObjectWithType* pObject = NULL;

    IFCPTR(ppRootObject);

    IFCEXPECT(m_ChildNode != NULL);

    pObject = m_ChildNode->GetObject();
    IFCPTR(pObject);

    *ppRootObject = pObject;
    AddRefObject(pObject);

Cleanup:
    return hr;
}

HRESULT CParserCallback::OnElementStart(CXMLElementStart* pElementStart)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementStart);

    if(m_ChildNode == NULL)
    {
        IFC(ElementStartToParserCallback(m_ParseContext, pElementStart, &m_ChildNode));
    }
    else
    {
        BOOL Consumed = FALSE;

        IFC(m_ChildNode->OnElementStart(pElementStart, Consumed));
    }

Cleanup:
    return hr;
}

HRESULT CParserCallback::OnElementEnd(CXMLElementEnd* pElementEnd)
{
    HRESULT hr = S_OK;
    BOOL Consumed = FALSE;

    IFCPTR(pElementEnd);

    IFCEXPECT(m_ChildNode != NULL);

    IFC(m_ChildNode->OnElementEnd(pElementEnd, Consumed));

Cleanup:
    return hr;
}

HRESULT CParserCallback::OnText(CXMLText* pText)
{
    HRESULT hr = S_OK;
    BOOL Consumed = FALSE;

    IFCPTR(pText);

    IFCEXPECT(m_ChildNode != NULL);

    IFC(m_ChildNode->OnText(pText, Consumed));

Cleanup:
    return hr;
}

HRESULT CParserCallback::OnAttribute(CXMLAttribute* pAttribute)
{
    HRESULT hr = S_OK;
    BOOL Consumed = FALSE;

    IFCPTR(pAttribute);

    IFCEXPECT(m_ChildNode != NULL);

    IFC(m_ChildNode->OnAttribute(pAttribute, Consumed));

Cleanup:
    return hr;
}