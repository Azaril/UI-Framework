#include "ControlTemplate.h"
#include "FrameworkElement.h"

//
// Properties
//
CStaticProperty CControlTemplate::TemplateProperty( L"Template", TypeIndex::ParserCommandList, StaticPropertyFlags::Content );

CControlTemplate::CControlTemplate() : m_TemplateCommandList(NULL)
{
}

CControlTemplate::~CControlTemplate()
{
    ReleaseObject(m_TemplateCommandList);
}

HRESULT CControlTemplate::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

Cleanup:
    return hr;
}

HRESULT CControlTemplate::LoadContent(CNamescope* pNamescope, CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;

    CControlTemplateParseCallback Callback(pNamescope);

    IFCPTR(m_TemplateCommandList);

    IFC(m_TemplateCommandList->Execute(&Callback, ppObject));

Cleanup:
    return hr;
}

HRESULT CControlTemplate::CreatePropertyInformation(CPropertyInformation **ppInformation)
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

HRESULT CControlTemplate::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
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

HRESULT CControlTemplate::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
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
        IFC(CPropertyObject::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}




CControlTemplateParseCallback::CControlTemplateParseCallback(CNamescope* pNamescope) : m_Namescope(pNamescope)
{
}

HRESULT CControlTemplateParseCallback::OnPushObject(CObjectWithType* pObject)
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CControlTemplateParseCallback::OnPopObject(CObjectWithType* pObject)
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