#include "ParserNodeCallback.h"

CParserNodeCallback::CParserNodeCallback(
	) 
	: m_Context(NULL)
{
}

CParserNodeCallback::~CParserNodeCallback(
	)
{
    ReleaseObject(m_Context);
}

__checkReturn HRESULT 
CParserNodeCallback::Initialize(
	__in CParseContext* pContext
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pContext);

    m_Context = pContext;
    AddRefObject(m_Context);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CParserNodeCallback::OnElementStart(
	__in CXMLElementStart* pElementStart, 
	BOOL& Consumed
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementStart);

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CParserNodeCallback::OnElementEnd(
	__in CXMLElementEnd* pElementEnd,
	BOOL& Consumed
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementEnd);

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CParserNodeCallback::OnText(
	__in CXMLText* pText,
	BOOL& Consumed
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CParserNodeCallback::OnAttribute(
	__in CXMLAttribute* pAttribute,
	BOOL& Consumed
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pAttribute);

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}