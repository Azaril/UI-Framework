#include "ControlTemplate.h"

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

HRESULT CControlTemplate::LoadContent(CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;

    IFCPTR(m_TemplateCommandList);

    IFC(m_TemplateCommandList->Execute(ppObject));

Cleanup:
    return hr;
}

HRESULT CControlTemplate::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &TemplateProperty
    };

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