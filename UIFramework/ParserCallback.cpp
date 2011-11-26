#include "ParserCallback.h"
#include "BasicTypes.h"
#include "ParserUtilities.h"

CParserCallback::CParserCallback(
	) 
	: m_ChildNode(NULL)
	, m_ParseContext(NULL)
{
}

CParserCallback::~CParserCallback(
	)
{
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_ParseContext);
}

__checkReturn HRESULT 
CParserCallback::Initialize(
	__in CParseContext* pContext
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pContext);

    m_ParseContext = pContext;
    AddRefObject(m_ParseContext);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CParserCallback::OnElementStart(
	__in CXMLElementStart* pElementStart
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementStart);

    if(m_ChildNode == NULL)
    {
        IFC(ElementStartToParserCallback(m_ParseContext, pElementStart, &m_ChildNode));
    }
    else
    {
        bool Consumed = FALSE;

        IFC(m_ChildNode->OnElementStart(pElementStart, Consumed));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CParserCallback::OnElementEnd(
	__in CXMLElementEnd* pElementEnd
	)
{
    HRESULT hr = S_OK;
    bool Consumed = FALSE;

    IFCPTR(pElementEnd);

    IFCEXPECT(m_ChildNode != NULL);

    IFC(m_ChildNode->OnElementEnd(pElementEnd, Consumed));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CParserCallback::OnText(
	__in CXMLText* pText
	)
{
    HRESULT hr = S_OK;
    bool Consumed = FALSE;

    IFCPTR(pText);

    IFCEXPECT(m_ChildNode != NULL);

    IFC(m_ChildNode->OnText(pText, Consumed));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CParserCallback::OnAttribute(
	__in CXMLAttribute* pAttribute
	)
{
    HRESULT hr = S_OK;
    bool Consumed = FALSE;

    IFCPTR(pAttribute);

    IFCEXPECT(m_ChildNode != NULL);

    IFC(m_ChildNode->OnAttribute(pAttribute, Consumed));

Cleanup:
    return hr;
}