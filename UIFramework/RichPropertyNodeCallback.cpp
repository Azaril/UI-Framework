#include "RichPropertyNodeCallback.h"
#include "ParserUtilities.h"
#include "BasicTypes.h"

CRichPropertyNodeCallback::CRichPropertyNodeCallback() : m_Parent(NULL),
                                                         m_Complete(FALSE),
                                                         m_ChildNode(NULL),
                                                         m_ResolvedClass(NULL),
                                                         m_Properties(NULL),
                                                         m_Property(NULL),
                                                         m_SetTextValue(FALSE),
                                                         m_SetObjectValue(FALSE)
{
}

CRichPropertyNodeCallback::~CRichPropertyNodeCallback()
{
    ReleaseObject(m_Parent);
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_Property);
    ReleaseObject(m_ResolvedClass);
    ReleaseObject(m_Properties);
}

HRESULT CRichPropertyNodeCallback::Initialize(CParseContext* pContext, CPropertyObject* pParent, CXMLElementStart* pXMLStart)
{
    HRESULT hr = S_OK;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;
    const WCHAR* pPropertyStart = NULL;
    CPropertyInformation* pProperties = NULL;
    WCHAR* pClassType = NULL;
    UINT32 ClassTypeLength = 0;

    IFCPTR(pContext);
    IFCPTR(pParent);
    IFCPTR(pXMLStart);

    IFC(CParserNodeCallback::Initialize(pContext));

    m_Parent = pParent;
    AddRefObject(m_Parent);

    IFC(pXMLStart->GetName(&pElementName, &ElementNameLength));

    pPropertyStart = wcschr(pElementName, L'.');
    IFCPTR(pPropertyStart);

    ClassTypeLength = (pPropertyStart - pElementName);

    pClassType = new WCHAR[ClassTypeLength + 1];
    IFCOOM(pClassType);

    wcsncpy_s(pClassType, ClassTypeLength + 1, pElementName, ClassTypeLength);
    pClassType[ClassTypeLength] = L'\0';

    ++pPropertyStart;

    IFC(m_Context->GetClassResolver()->ResolveType(pClassType, &m_ResolvedClass));

    IFC(m_ResolvedClass->GetPropertyInformation(&m_Properties));

    IFC(m_Properties->GetProperty(pPropertyStart, &m_Property));

Cleanup:
    ReleaseObject(pProperties);
    delete [] pClassType;

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

        IFC(ElementStartToParserCallback(m_Context, m_Parent, pElementStart, &m_ChildNode));

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
            IFC(AssignProperty(m_Parent, m_Property, m_ChildNode->GetObject(), m_Context->GetTypeConverter()));

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

        IFC(AssignProperty(m_Parent, m_Property, pStringValue, m_Context->GetTypeConverter()))

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