#include "RichPropertyNodeCallback.h"
#include "ParserUtilities.h"
#include "BasicTypes.h"

CRichPropertyNodeCallback::CRichPropertyNodeCallback() : m_Complete(FALSE),
                                                         m_ChildNode(NULL),
                                                         m_Property(NULL),
                                                         m_SetTextValue(FALSE),
                                                         m_SetObjectValue(FALSE)
{
}

CRichPropertyNodeCallback::~CRichPropertyNodeCallback()
{
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_Property);
}

HRESULT CRichPropertyNodeCallback::Initialize(CParseContext* pContext, CPropertyInformation* pProperties, CXMLElementStart* pXMLStart)
{
    HRESULT hr = S_OK;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;

    IFCPTR(pContext);
    IFCPTR(pXMLStart);

    IFC(CParserNodeCallback::Initialize(pContext));

    IFC(pXMLStart->GetName(&pElementName, &ElementNameLength));

    IFC(m_Context->GetClassResolver()->ResolveProperty(pElementName, pProperties, &m_Property));

Cleanup:

    return hr;
}

HRESULT CRichPropertyNodeCallback::OnElementStart(CXMLElementStart* pElementStart, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementStart);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnElementStart(pElementStart, Consumed));
    }
    else
    {
        IFCEXPECT(!m_SetTextValue);

        IFC(ElementStartToParserCallback(m_Context, pElementStart, &m_ChildNode));

        m_SetObjectValue = TRUE;

        Consumed = TRUE;
    }

Cleanup:
    return hr;
}

HRESULT CRichPropertyNodeCallback::OnElementEnd(CXMLElementEnd* pElementEnd, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementEnd);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnElementEnd(pElementEnd, Consumed));

        if(m_ChildNode->IsComplete())
        {
            IFC(AddSetPropertyCommand(m_Context, m_Property, m_ChildNode->GetKey()));

            ReleaseObject(m_ChildNode);
        }
    }
    
    if(!Consumed)
    {
        Consumed = TRUE;

        m_Complete = TRUE;
    }

Cleanup:
    return hr;
}

HRESULT CRichPropertyNodeCallback::OnText(CXMLText* pText, BOOL& Consumed)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    IFCPTR(pText);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnText(pText, Consumed));
    }
    else
    {
        const WCHAR* pString = NULL;
        UINT32 StringLength = 0;

        IFCEXPECT(!m_Complete);
        IFCEXPECT(!m_SetTextValue);
        IFCEXPECT(!m_SetObjectValue);

        IFC(pText->GetText(&pString, &StringLength));

        IFC(CStringValue::Create(pString, StringLength, &pStringValue));

        IFC(AddPushValueCommand(m_Context, pStringValue));

        IFC(AddSetPropertyCommand(m_Context, m_Property));

        m_SetTextValue = TRUE;

        Consumed = TRUE;
    }

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

HRESULT CRichPropertyNodeCallback::OnAttribute(CXMLAttribute* pAttribute, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pAttribute);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnAttribute(pAttribute, Consumed));
    }
    else
    {
        IFC(E_UNEXPECTED);
    }

Cleanup:
    return hr;
}

BOOL CRichPropertyNodeCallback::IsComplete()
{
    return m_Complete;
}