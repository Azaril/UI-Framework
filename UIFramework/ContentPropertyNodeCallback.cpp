#include "ContentPropertyNodeCallback.h"
#include "ParserUtilities.h"
#include "BasicTypes.h"

CContentPropertyNodeCallback::CContentPropertyNodeCallback(
	) 
	: m_ChildNode(NULL)
	, m_Complete(FALSE)
	, m_Property(NULL)
	, m_SetTextValue(FALSE)
	, m_SetObjectValue(FALSE)
	, m_IsTemplate(FALSE)
	, m_ChildCommandList(NULL)
{
}

CContentPropertyNodeCallback::~CContentPropertyNodeCallback(
	)
{
    ReleaseObject(m_ChildCommandList);
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_Property);
}

__checkReturn HRESULT 
CContentPropertyNodeCallback::Initialize(
	__in CParseContext* pContext, 
	__in CPropertyInformation* pPropertyInformation
	)
{
    HRESULT hr = S_OK;
    CPropertyInformation* pProperties = NULL;

    IFCPTR(pContext);
    IFCPTR(pPropertyInformation);

    IFC(CParserNodeCallback::Initialize(pContext));

    IFC(pPropertyInformation->GetContentProperty(&m_Property));

    m_IsTemplate = (m_Property->GetType() == TypeIndex::ParserCommandList);

    if(m_IsTemplate)
    {
        IFC(CParserCommandList::Create(pContext->GetProviders(), &m_ChildCommandList));

        IFC(pContext->PushCommandList(m_ChildCommandList))
    }

Cleanup:
    ReleaseObject(pProperties);

    return hr;
}

__checkReturn HRESULT 
CContentPropertyNodeCallback::Initialize(
	__in CParseContext* pContext, 
	__in CPropertyInformation* pPropertyInformation, 
	__in CXMLElementStart* pXMLStart
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pContext);
    IFCPTR(pXMLStart);

    IFC(Initialize(pContext, pPropertyInformation));

    IFC(ElementStartToParserCallback(pContext, pXMLStart, &m_ChildNode));

    m_SetObjectValue = TRUE;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CContentPropertyNodeCallback::Initialize(
	__in CParseContext* pContext, 
	__in CPropertyInformation* pPropertyInformation, 
	__in CXMLText* pXMLText
	)
{
    HRESULT hr = S_OK;
    bool Consumed = FALSE;

    IFCPTR(pContext);
    IFCPTR(pXMLText);

    IFC(Initialize(pContext, pPropertyInformation));

    IFC(OnText(pXMLText, Consumed));

    IFCEXPECT(Consumed);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CContentPropertyNodeCallback::OnElementStart(
	__in CXMLElementStart* pElementStart, 
	bool& Consumed
	)
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
        IFCEXPECT(!m_SetObjectValue || m_Property->IsCollection() || m_Property->IsDictionary());

        IFC(ElementStartToParserCallback(m_Context, pElementStart, &m_ChildNode));

        m_SetObjectValue = TRUE;

        Consumed = TRUE;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CContentPropertyNodeCallback::OnElementEnd(
	__in CXMLElementEnd* pElementEnd, 
	bool& Consumed
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementEnd);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnElementEnd(pElementEnd, Consumed));

        if(m_ChildNode->IsComplete())
        {
            if(m_IsTemplate)
            {
                IFC(m_Context->PopCommandList());

                IFC(AddPushValueCommand(m_Context, m_ChildCommandList));
            }

            IFC(AddSetPropertyCommand(m_Context, m_Property, m_ChildNode->GetKey()));

            ReleaseObject(m_ChildNode);
        }
    }
    
    if(!Consumed)
    {
        //NOTE: Don't consume end tag.

        m_Complete = TRUE;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CContentPropertyNodeCallback::OnText(
	__in CXMLText* pText, 
	bool& Consumed
	)
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
        IFCEXPECT(!m_SetTextValue);
        IFCEXPECT(!m_SetObjectValue);

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

__checkReturn HRESULT
CContentPropertyNodeCallback::OnAttribute(
	__in CXMLAttribute* pAttribute,
	bool& Consumed
	)
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

bool 
CContentPropertyNodeCallback::IsComplete(
	)
{
    return m_Complete;
}