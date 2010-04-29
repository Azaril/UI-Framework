#include "ParserCallback.h"
#include "BasicTypes.h"

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

HRESULT CParserCallback::GetRootElement(CUIElement** ppRootElement)
{
    HRESULT hr = S_OK;

    IFCPTR(ppRootElement);

    IFCEXPECT(m_ChildNode != NULL);

    *ppRootElement = m_ChildNode->GetUIElement();
    AddRefObject(*ppRootElement);

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



BOOL IsAttribute(const WCHAR* pText)
{
    if(pText != NULL)
    {
        return (wcschr(pText, L'.') != NULL);
    }

    return FALSE;
}

HRESULT ElementStartToParserCallback(CParseContext* pContext, CXMLElementStart* pStart, CElementNodeCallback** ppCallback)
{
    HRESULT hr = S_OK;

    IFCPTR(pContext);
    IFCPTR(pStart);
    IFCPTR(ppCallback);

    IFC(ElementStartToParserCallback(pContext, NULL, pStart, ppCallback));

Cleanup:
    return hr;
}

HRESULT ElementStartToParserCallback(CParseContext* pContext, CUIElement* pParent, CXMLElementStart* pStart, CElementNodeCallback** ppCallback)
{
    HRESULT hr = S_OK;
    CElementNodeCallback* pElementCallback = NULL;

    IFCPTR(pContext);
    IFCPTR(pStart);
    IFCPTR(ppCallback);

    IFC(CElementNodeCallback::Create(pContext, pParent, pStart, &pElementCallback));

    *ppCallback = pElementCallback;
    pElementCallback = NULL;

Cleanup:
    ReleaseObject(pElementCallback);

    return hr;
}

HRESULT ElementStartToParserCallback(CParseContext* pContext, CUIElement* pParent, CXMLElementStart* pStart, CPropertyCallback** ppCallback)
{
    HRESULT hr = S_OK;
    CRichPropertyNodeCallback* pRichPropertyCallback = NULL;
    CContentPropertyNodeCallback* pContentPropertyCallback = NULL;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;

    IFCPTR(pStart);
    IFCPTR(pParent);
    IFCPTR(ppCallback);

    IFC(pStart->GetName(&pElementName, &ElementNameLength));

    if(IsAttribute(pElementName))
    {
        IFC(CRichPropertyNodeCallback::Create(pContext, pParent, pStart, &pRichPropertyCallback));

        *ppCallback = pRichPropertyCallback;
        pRichPropertyCallback = NULL;
    }
    else
    {
        IFC(CContentPropertyNodeCallback::Create(pContext, pParent, pStart, &pContentPropertyCallback));

        *ppCallback = pContentPropertyCallback;
        pContentPropertyCallback = NULL;
    }

Cleanup:
    ReleaseObject(pRichPropertyCallback);
    ReleaseObject(pContentPropertyCallback);

    return hr;
}

HRESULT TextToParserCallback(CParseContext* pContext, CUIElement* pParent, CXMLText* pText, CPropertyCallback** ppCallback)
{
    HRESULT hr = S_OK;
    CContentPropertyNodeCallback* pContentPropertyCallback = NULL;

    IFC(CContentPropertyNodeCallback::Create(pContext, pParent, pText, &pContentPropertyCallback));

    *ppCallback = pContentPropertyCallback;
    pContentPropertyCallback = NULL;

Cleanup:
    ReleaseObject(pContentPropertyCallback);

    return hr;
}


CParseContext::CParseContext() : m_ClassResolver(NULL),
                                 m_TypeConverter(NULL)
{
}

CParseContext::~CParseContext()
{
    ReleaseObject(m_ClassResolver);
}

HRESULT CParseContext::Initialize(CClassResolver* pClassResolver, CTypeConverter* pTypeConverter)
{
    HRESULT hr = S_OK;

    IFCPTR(pClassResolver);
    IFCPTR(pTypeConverter);

    m_ClassResolver = pClassResolver;
    AddRefObject(m_ClassResolver);

    m_TypeConverter = pTypeConverter;
    AddRefObject(m_TypeConverter);

Cleanup:
    return hr;
}

CClassResolver* CParseContext::GetClassResolver()
{
    return m_ClassResolver;
}

CTypeConverter* CParseContext::GetTypeConverter()
{
    return m_TypeConverter;
}


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


CElementNodeCallback::CElementNodeCallback() : m_Element(NULL),
                                               m_ChildNode(NULL),
                                               m_Complete(FALSE)
{
}

CElementNodeCallback::~CElementNodeCallback()
{
    ReleaseObject(m_Element);
    ReleaseObject(m_ChildNode);
}

HRESULT CElementNodeCallback::Initialize(CParseContext* pContext, CUIElement* pParent, CXMLElementStart* pXMLStart)
{
    HRESULT hr = S_OK;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;
    ClassType ElementType = { 0 };

    IFCPTR(pContext);
    IFCPTR(pXMLStart);

    IFC(CParserNodeCallback::Initialize(pContext));

    IFC(pXMLStart->GetName(&pElementName, &ElementNameLength));

    IFC(m_Context->GetClassResolver()->ResolveType(pElementName, &ElementType));

    IFCPTR(ElementType.CreateType);

    IFC(ElementType.CreateType(&m_Element));

Cleanup:
    return hr;
}

CUIElement* CElementNodeCallback::GetUIElement()
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
        IFC(ElementStartToParserCallback(m_Context, m_Element, pElementStart, &m_ChildNode));

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
        IFC(TextToParserCallback(m_Context, m_Element, pText, &m_ChildNode));

        Consumed = TRUE;
    }

Cleanup:
    return hr;
}

HRESULT CElementNodeCallback::OnAttribute(CXMLAttribute* pAttribute, BOOL& Consumed)
{
    HRESULT hr = S_OK;
    CPropertyInformation* pProperties = NULL;
    CProperty* pProperty = NULL;
    CStringValue* pStringValue = NULL;

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

        IFCEXPECT(!m_Complete);

        IFC(pAttribute->GetName(&pNameString, &NameStringLength));
        IFC(pAttribute->GetValue(&pValueString, &ValueStringLength));

        pPropertyStart = wcschr(pNameString, L'.');
        
        if(pPropertyStart == NULL)
        {
            pPropertyStart = pNameString;
        }
        else
        {
            ++pPropertyStart;
        }

        IFCPTR(pPropertyStart);

        IFC(m_Element->GetPropertyInformation(&pProperties));

        IFC(pProperties->GetProperty(pPropertyStart, &pProperty));

        IFC(CStringValue::Create(pValueString, ValueStringLength, &pStringValue));

        IFC(AssignProperty(m_Element, pProperty, pStringValue, m_Context->GetTypeConverter()))

        Consumed = TRUE;
    }

Cleanup:
    ReleaseObject(pProperties);
    ReleaseObject(pProperty);
    ReleaseObject(pStringValue);

    return hr;
}

BOOL CElementNodeCallback::IsComplete()
{
    return m_Complete;
}



CRichPropertyNodeCallback::CRichPropertyNodeCallback() : m_Parent(NULL),
                                                         m_Complete(FALSE),
                                                         m_ChildNode(NULL),
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
}

HRESULT CRichPropertyNodeCallback::Initialize(CParseContext* pContext, CUIElement* pParent, CXMLElementStart* pXMLStart)
{
    HRESULT hr = S_OK;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;
    const WCHAR* pPropertyStart = NULL;
    CPropertyInformation* pProperties = NULL;

    IFCPTR(pContext);
    IFCPTR(pParent);
    IFCPTR(pXMLStart);

    IFC(CParserNodeCallback::Initialize(pContext));

    m_Parent = pParent;
    AddRefObject(m_Parent);

    IFC(pXMLStart->GetName(&pElementName, &ElementNameLength));

    pPropertyStart = wcschr(pElementName, L'.');
    IFCPTR(pPropertyStart);

    ++pPropertyStart;

    IFC(pParent->GetPropertyInformation(&pProperties));

    IFC(pProperties->GetProperty(pPropertyStart, &m_Property));

Cleanup:
    ReleaseObject(pProperties);

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
    }

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

BOOL CRichPropertyNodeCallback::IsComplete()
{
    return m_Complete;
}



CContentPropertyNodeCallback::CContentPropertyNodeCallback() : m_ChildNode(NULL),
                                                               m_Complete(FALSE),
                                                               m_Parent(NULL),
                                                               m_Property(NULL),
                                                               m_SetTextValue(FALSE),
                                                               m_SetObjectValue(FALSE)
{
}

CContentPropertyNodeCallback::~CContentPropertyNodeCallback()
{
    ReleaseObject(m_Parent);
    ReleaseObject(m_ChildNode);
    ReleaseObject(m_Property);
}

HRESULT CContentPropertyNodeCallback::Initialize(CParseContext* pContext, CUIElement* pParent)
{
    HRESULT hr = S_OK;
    CPropertyInformation* pProperties = NULL;

    IFCPTR(pContext);
    IFCPTR(pParent);

    IFC(CParserNodeCallback::Initialize(pContext));

    IFC(pParent->GetPropertyInformation(&pProperties));

    IFC(pProperties->GetContentProperty(&m_Property));

    m_Parent = pParent;
    AddRefObject(m_Parent);

Cleanup:
    ReleaseObject(pProperties);

    return hr;
}

HRESULT CContentPropertyNodeCallback::Initialize(CParseContext* pContext, CUIElement* pParent, CXMLElementStart* pXMLStart)
{
    HRESULT hr = S_OK;

    IFCPTR(pContext);
    IFCPTR(pParent);
    IFCPTR(pXMLStart);

    IFC(Initialize(pContext, pParent));

    IFC(ElementStartToParserCallback(pContext, pXMLStart, &m_ChildNode));

    m_SetObjectValue = TRUE;

Cleanup:
    return hr;
}

HRESULT CContentPropertyNodeCallback::Initialize(CParseContext* pContext, CUIElement* pParent, CXMLText* pXMLText)
{
    HRESULT hr = S_OK;
    BOOL Consumed = FALSE;

    IFCPTR(pContext);
    IFCPTR(pParent);
    IFCPTR(pXMLText);

    IFC(Initialize(pContext, pParent));

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
            IFC(AssignProperty(m_Parent, m_Property, m_ChildNode->GetUIElement(), m_Context->GetTypeConverter()));

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

        IFC(E_UNEXPECTED);
    }

Cleanup:
    return hr;
}

BOOL CContentPropertyNodeCallback::IsComplete()
{
    return m_Complete;
}

HRESULT AssignProperty(CUIElement* pElement, CProperty* pProperty, CObjectWithType* pValue, CTypeConverter* pTypeConverter)
{
    HRESULT hr = S_OK;
    CObjectWithType* pConvertedType = NULL;

    IFCPTR(pElement);
    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty->GetType() == pValue->GetType() || pValue->IsTypeOf(pProperty->GetType()))
    {
        IFC(pElement->SetValue(pProperty, pValue));
    }
    else
    {
        IFCPTR(pTypeConverter);

        IFC(pTypeConverter->Convert(pValue, pProperty->GetType(), &pConvertedType));

        IFC(pElement->SetValue(pProperty, pConvertedType));
    }

Cleanup:
    ReleaseObject(pConvertedType);

    return hr;
}