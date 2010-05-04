#include "ContentPropertyNodeCallback.h"
#include "ParserUtilities.h"
#include "BasicTypes.h"

CContentPropertyNodeCallback::CContentPropertyNodeCallback() : m_ChildNode(NULL),
                                                               m_Complete(FALSE),
                                                               m_Parent(NULL),
                                                               m_Property(NULL),
                                                               m_SetTextValue(FALSE),
                                                               m_SetObjectValue(FALSE),
                                                               m_Properties(NULL)
{
}

CContentPropertyNodeCallback::~CContentPropertyNodeCallback()
{
    ReleaseObject(m_Parent);
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_Property);
    ReleaseObject(m_Properties);
}

HRESULT CContentPropertyNodeCallback::Initialize(CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation)
{
    HRESULT hr = S_OK;
    CPropertyInformation* pProperties = NULL;

    IFCPTR(pContext);
    IFCPTR(pParent);
    IFCPTR(pPropertyInformation);

    IFC(CParserNodeCallback::Initialize(pContext));

    m_Properties = pPropertyInformation;
    AddRefObject(m_Properties);

    IFC(pPropertyInformation->GetContentProperty(&m_Property));

    m_Parent = pParent;
    AddRefObject(m_Parent);

Cleanup:
    ReleaseObject(pProperties);

    return hr;
}

HRESULT CContentPropertyNodeCallback::Initialize(CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLElementStart* pXMLStart)
{
    HRESULT hr = S_OK;

    IFCPTR(pContext);
    IFCPTR(pParent);
    IFCPTR(pXMLStart);

    IFC(Initialize(pContext, pParent, pPropertyInformation));

    IFC(ElementStartToParserCallback(pContext, pXMLStart, &m_ChildNode));

    m_SetObjectValue = TRUE;

Cleanup:
    return hr;
}

HRESULT CContentPropertyNodeCallback::Initialize(CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLText* pXMLText)
{
    HRESULT hr = S_OK;
    BOOL Consumed = FALSE;

    IFCPTR(pContext);
    IFCPTR(pParent);
    IFCPTR(pXMLText);

    IFC(Initialize(pContext, pParent, pPropertyInformation));

    IFC(OnText(pXMLText, Consumed));

    IFCEXPECT(Consumed);

Cleanup:
    return hr;
}

HRESULT CContentPropertyNodeCallback::OnElementStart(CXMLElementStart* pElementStart, BOOL& Consumed)
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

        Consumed = TRUE;
    }

Cleanup:
    return hr;
}

HRESULT CContentPropertyNodeCallback::OnElementEnd(CXMLElementEnd* pElementEnd, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementEnd);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnElementEnd(pElementEnd, Consumed));

        if(m_ChildNode->IsComplete())
        {
            IFC(AssignProperty(m_Parent, m_Property, m_ChildNode->GetObject(), m_Context->GetTypeConverter()));

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

HRESULT CContentPropertyNodeCallback::OnText(CXMLText* pText, BOOL& Consumed)
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

        IFC(AssignProperty(m_Parent, m_Property, pStringValue, m_Context->GetTypeConverter()))

        m_SetTextValue = TRUE;

        Consumed = TRUE;
    }

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

HRESULT CContentPropertyNodeCallback::OnAttribute(CXMLAttribute* pAttribute, BOOL& Consumed)
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

BOOL CContentPropertyNodeCallback::IsComplete()
{
    return m_Complete;
}