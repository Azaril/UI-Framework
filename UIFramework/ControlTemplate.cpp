#include "ControlTemplate.h"
#include "FrameworkElement.h"
#include "LayeredValue.h"

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
{
}

CControlTemplate::~CControlTemplate(
    )
{
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
    CParserCommandList* pCommandList = NULL;

    CControlTemplateParseCallback Callback(pNamescope);

    IFC(GetTypedEffectiveValue(&TemplateProperty, &pCommandList));

    IFCPTR(pCommandList);

    IFC(pCommandList->Execute(&Callback, ppObject));

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
CControlTemplate::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::ControlTemplate)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case ControlTemplateProperties::Template:
                {
                    *ppLayeredValue = &m_TemplateCommandList;
                    break;
                }

            default:
                {
                    IFC(E_UNEXPECTED);
                }
        }
    }
    else
    {
        IFC_NOTRACE(CPropertyObject::GetLayeredValue(pProperty, ppLayeredValue));
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