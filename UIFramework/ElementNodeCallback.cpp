#include "ElementNodeCallback.h"
#include "ParserUtilities.h"
#include "BasicTypes.h"
#include "ParserCommand.h"
#include "CreateObjectCommand.h"

CElementNodeCallback::CElementNodeCallback() : m_ChildNode(NULL),
                                               m_ResolvedClass(NULL),
                                               m_Properties(NULL),
                                               m_Complete(FALSE),
                                               m_KeyString(NULL)
{
}

CElementNodeCallback::~CElementNodeCallback()
{
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_ResolvedClass);
    ReleaseObject(m_Properties);
    ReleaseObject(m_KeyString);
}

HRESULT CElementNodeCallback::Initialize(CParseContext* pContext, CXMLElementStart* pXMLStart)
{
    HRESULT hr = S_OK;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;

    IFCPTR(pContext);
    IFCPTR(pXMLStart);

    IFC(CParserNodeCallback::Initialize(pContext));

    IFC(pXMLStart->GetName(&pElementName, &ElementNameLength));

    IFC(m_Context->GetClassResolver()->ResolveType(pElementName, &m_ResolvedClass));

    IFC(AddCreateObjectCommand(pContext, m_ResolvedClass));

    IFC(m_ResolvedClass->GetPropertyInformation(&m_Properties));

Cleanup:
    return hr;
}

CStringValue* CElementNodeCallback::GetKey()
{
    return m_KeyString;
}

HRESULT CElementNodeCallback::OnElementStart(CXMLElementStart* pElementStart, BOOL& Consumed)
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
        IFC(ElementStartToParserCallback(m_Context, m_Properties, pElementStart, &m_ChildNode));

        Consumed = TRUE;
    }

Cleanup:
    return hr;
}

HRESULT CElementNodeCallback::OnElementEnd(CXMLElementEnd* pElementEnd, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pElementEnd);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnElementEnd(pElementEnd, Consumed));

        if(m_ChildNode->IsComplete())
        {
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

HRESULT CElementNodeCallback::OnText(CXMLText* pText, BOOL& Consumed)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnText(pText, Consumed));
    }
    else
    {
        IFC(TextToParserCallback(m_Context, m_Properties, pText, &m_ChildNode));

        Consumed = TRUE;
    }

Cleanup:
    return hr;
}

HRESULT CElementNodeCallback::OnAttribute(CXMLAttribute* pAttribute, BOOL& Consumed)
{
    HRESULT hr = S_OK;
    CProperty* pProperty = NULL;
    WCHAR* pClassType = NULL;
    UINT32 ClassTypeLength = 0;
    CStringValue* pAttributeValue = NULL;
    CParserCommandList* pParserCommandList = NULL;

    IFCPTR(pAttribute);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnAttribute(pAttribute, Consumed));
    }
    else
    {
        const WCHAR* pNamespaceUriString = NULL;
        UINT32 NamespaceUriStringLength = 0;
        const WCHAR* pNameString = NULL;
        UINT32 NameStringLength = 0;
        const WCHAR* pValueString = NULL;
        UINT32 ValueStringLength = 0;
        const WCHAR* pPropertyStart = NULL;
        CPropertyInformation* pPropertyLookup = NULL;

        IFCEXPECT(!m_Complete);

        IFC(pAttribute->GetNamespaceUri(&pNamespaceUriString, &NamespaceUriStringLength));
        IFC(pAttribute->GetName(&pNameString, &NameStringLength));
        IFC(pAttribute->GetValue(&pValueString, &ValueStringLength));

        if(wcscmp(pNamespaceUriString, L"http://www.w3.org/2000/xmlns/") == 0)
        {
            Consumed = TRUE;
        }
        else if(wcscmp(pNamespaceUriString, L"http://internal") == 0)
        {
            if(wcscmp(pNameString, L"Key") == 0)
            {
                IFC(CStringValue::Create(pValueString, ValueStringLength, &m_KeyString));
            }
            else
            {
                IFC(E_UNEXPECTED);
            }

            Consumed = TRUE;
        }
        else
        {
            IFC(m_Context->GetClassResolver()->ResolveProperty(pNameString, m_Properties, &pProperty));

            BOOL IsTemplate = (pProperty->GetType() == TypeIndex::ParserCommandList);

            if(IsTemplate)
            {
                IFC(CParserCommandList::Create(m_Context->GetProviders(), &pParserCommandList));

                IFC(m_Context->PushCommandList(pParserCommandList))
            }

            IFC(CStringValue::Create(pValueString, ValueStringLength, &pAttributeValue));

            IFC(EvaluateAndAddAttribute(m_Context, pValueString, ValueStringLength));

            if(IsTemplate)
            {
                IFC(m_Context->PopCommandList());

                IFC(AddPushValueCommand(m_Context, pParserCommandList));
            }

            IFC(AddSetPropertyCommand(m_Context, pProperty));

            Consumed = TRUE;
        }
    }

Cleanup:
    ReleaseObject(pProperty);
    ReleaseObject(pAttributeValue);
    ReleaseObject(pParserCommandList);

    delete [] pClassType;

    return hr;
}

BOOL CElementNodeCallback::IsComplete()
{
    return m_Complete;
}