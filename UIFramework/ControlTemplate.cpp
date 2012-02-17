#include "ControlTemplate.h"
#include "FrameworkElement.h"

//
// Properties
//
namespace ControlTemplateProperties
{
    enum Value
    {
        Template
    };
}

//
// Properties
//
CStaticProperty CControlTemplate::TemplateProperty(TypeIndex::ControlTemplate, ControlTemplateProperties::Template, L"Template", TypeIndex::ParserCommandList, StaticPropertyFlags::Content);

CControlTemplate::CControlTemplate(
    ) 
    : m_TemplateCommandList(NULL)
{
}

CControlTemplate::~CControlTemplate(
    )
{
    ReleaseObject(m_TemplateCommandList);
}

__checkReturn HRESULT 
CControlTemplate::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CControlTemplate::LoadContent(
    __in CNamescope* pNamescope, 
    __deref_out CObjectWithType** ppObject
    )
{
    HRESULT hr = S_OK;

    CControlTemplateParseCallback Callback(pNamescope);

    IFCPTR(m_TemplateCommandList);

    IFC(m_TemplateCommandList->Execute(&Callback, ppObject));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CControlTemplate::CreatePropertyInformation(
    __deref_out CPropertyInformation **ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &TemplateProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

__override __checkReturn HRESULT 
CControlTemplate::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CControlTemplate::TemplateProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::ParserCommandList));

        ReleaseObject(m_TemplateCommandList);

        m_TemplateCommandList = (CParserCommandList*)pValue;

        AddRefObject(m_TemplateCommandList);
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CControlTemplate::GetValueInternal(
    __in CProperty* pProperty,
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CControlTemplate::TemplateProperty)
    {
        *ppValue = m_TemplateCommandList;
        AddRefObject(m_TemplateCommandList);
    }
    else
    {
        IFC(CPropertyObject::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

CControlTemplateParseCallback::CControlTemplateParseCallback(
    __in CNamescope* pNamescope
    ) 
    : m_Namescope(pNamescope)
{
}

__override __checkReturn HRESULT 
CControlTemplateParseCallback::OnPushObject(
    __in CObjectWithType* pObject
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__override __checkReturn HRESULT 
CControlTemplateParseCallback::OnPopObject(
    __in CObjectWithType* pObject
    )
{
    HRESULT hr = S_OK;
    CFrameworkElement* pElement = NULL;

    IFCPTR(pObject);

    if(pObject->IsTypeOf(TypeIndex::FrameworkElement))
    {
        IFC(CastType(pObject, &pElement));

        pElement->SetAutomaticNamescopeParticipation(FALSE);

        if(m_Namescope)
        {
            IFC(pElement->RegisterInNamescope(m_Namescope, NULL));
        }
    }

Cleanup:
    return hr;
}