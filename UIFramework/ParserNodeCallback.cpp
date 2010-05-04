#include "ParserNodeCallback.h"

CParserNodeCallback::CParserNodeCallback() : m_Context(NULL)
{
}

CParserNodeCallback::~CParserNodeCallback()
{
    ReleaseObject(m_Context);
}

HRESULT CParserNodeCallback::Initialize(CParseContext* pContext)
{
    HRESULT hr = S_OK;

    IFCPTR(pContext);

    m_Context = pContext;
    AddRefObject(m_Context);

Cleanup:
    return hr;
}

HRESULT CParserNodeCallback::OnElementStart(CXMLElementStart* pElementStart, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementStart);

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

HRESULT CParserNodeCallback::OnElementEnd(CXMLElementEnd* pElementEnd, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementEnd);

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

HRESULT CParserNodeCallback::OnText(CXMLText* pText, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

HRESULT CParserNodeCallback::OnAttribute(CXMLAttribute* pAttribute, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pAttribute);

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}