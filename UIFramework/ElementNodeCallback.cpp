#include "ElementNodeCallback.h"
#include "ParserUtilities.h"
#include "BasicTypes.h"

CElementNodeCallback::CElementNodeCallback() : m_Element(NULL),
                                               m_ChildNode(NULL),
                                               m_ResolvedClass(NULL),
                                               m_Properties(NULL),
                                               m_Complete(FALSE)
{
}

CElementNodeCallback::~CElementNodeCallback()
{
    ReleaseObject(m_Element);
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_ResolvedClass);
    ReleaseObject(m_Properties);
}

HRESULT CElementNodeCallback::Initialize(CParseContext* pContext, CPropertyObject* pParent, CXMLElementStart* pXMLStart)
{
    HRESULT hr = S_OK;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;

    IFCPTR(pContext);
    IFCPTR(pXMLStart);

    IFC(CParserNodeCallback::Initialize(pContext));

    IFC(pXMLStart->GetName(&pElementName, &ElementNameLength));

    IFC(m_Context->GetClassResolver()->ResolveType(pElementName, &m_ResolvedClass));

    IFC(m_ResolvedClass->CreateInstance(&m_Element));

    IFC(m_ResolvedClass->GetPropertyInformation(&m_Properties));

Cleanup:
    return hr;
}

CPropertyObject* CElementNodeCallback::GetObject()
{
    return m_Element;
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
        IFC(ElementStartToParserCallback(m_Context, m_Element, m_Properties, pElementStart, &m_ChildNode));

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
        IFC(TextToParserCallback(m_Context, m_Element, m_Properties, pText, &m_ChildNode));

        Consumed = TRUE;
    }

Cleanup:
    return hr;
}

HRESULT CElementNodeCallback::OnAttribute(CXMLAttribute* pAttribute, BOOL& Consumed)
{
    HRESULT hr = S_OK;
    CProperty* pProperty = NULL;
    CStringValue* pStringValue = NULL;
    WCHAR* pClassType = NULL;
    UINT32 ClassTypeLength = 0;
    CResolvedClass* pResolvedClass = NULL;
    CPropertyInformation* pResolvedClassProperties = NULL;

    IFCPTR(pAttribute);

    if(m_ChildNode)
    {
        IFCEXPECT(!m_ChildNode->IsComplete());

        IFC(m_ChildNode->OnAttribute(pAttribute, Consumed));
    }
    else
    {
        const WCHAR* pNameString = NULL;
        UINT32 NameStringLength = 0;
        const WCHAR* pValueString = NULL;
        UINT32 ValueStringLength = 0;
        const WCHAR* pPropertyStart = NULL;
        CPropertyInformation* pPropertyLookup = NULL;

        IFCEXPECT(!m_Complete);

        IFC(pAttribute->GetName(&pNameString, &NameStringLength));
        IFC(pAttribute->GetValue(&pValueString, &ValueStringLength));

        pPropertyStart = wcschr(pNameString, L'.');
        
        if(pPropertyStart == NULL)
        {
            pPropertyStart = pNameString;

            pPropertyLookup = m_Properties;
        }
        else
        {
            ClassTypeLength = (pPropertyStart - pNameString);

            pClassType = new WCHAR[ClassTypeLength + 1];
            IFCOOM(pClassType);

            wcsncpy_s(pClassType, ClassTypeLength + 1, pNameString, ClassTypeLength);
            pClassType[ClassTypeLength] = L'\0';

            IFC(m_Context->GetClassResolver()->ResolveType(pClassType, &pResolvedClass));

            IFC(pResolvedClass->GetPropertyInformation(&pResolvedClassProperties));

            pPropertyLookup = pResolvedClassProperties;

            ++pPropertyStart;
        }

        IFCPTR(pPropertyStart);

        IFC(pPropertyLookup->GetProperty(pPropertyStart, &pProperty));

        IFC(CStringValue::Create(pValueString, ValueStringLength, &pStringValue));

        IFC(AssignProperty(m_Element, pProperty, pStringValue, m_Context->GetTypeConverter()))

        Consumed = TRUE;
    }

Cleanup:
    ReleaseObject(pProperty);
    ReleaseObject(pStringValue);
    ReleaseObject(pResolvedClass);
    ReleaseObject(pResolvedClassProperties);
    delete [] pClassType;

    return hr;
}

BOOL CElementNodeCallback::IsComplete()
{
    return m_Complete;
}