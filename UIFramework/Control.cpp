#include "Control.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Template );

//
// Properties
//
CStaticProperty CControl::TemplateProperty( L"Template", TypeIndex::ControlTemplate, StaticPropertyFlags::None, &GET_DEFAULT( Template ), &INSTANCE_CHANGE_CALLBACK( CControl, OnTemplateChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CControl, OnTemplateChanged );

CControl::CControl() : m_Template(this, &CControl::TemplateProperty),
                       m_TemplateDirty(TRUE),
                       m_TemplateChild(NULL)
{
}

CControl::~CControl()
{
    ReleaseObject(m_TemplateChild);
}

HRESULT CControl::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize());

Cleanup:
    return hr;
}

HRESULT CControl::OnTemplateChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    m_TemplateDirty = TRUE;

    IFC(RevokeTemplate());

    IFC(EnsureTemplate());

Cleanup:
    return hr;
}

HRESULT CControl::RevokeTemplate()
{
    HRESULT hr = S_OK;

    if(m_TemplateChild != NULL)
    {
        IFC(RemoveLogicalChild(m_TemplateChild));

        ReleaseObject(m_TemplateChild);
    }

Cleanup:
    return hr;
}

HRESULT CControl::EnsureTemplate()
{
    HRESULT hr = S_OK;
    CControlTemplate* pTemplate = NULL;

    if(m_TemplateDirty)
    {
        if(IsAttached())
        {
            IFC(RevokeTemplate());

            IFC(GetEffectiveTemplate(&pTemplate));

            if(pTemplate)
            {
                IFC(ApplyTemplate(pTemplate));
            }

            m_TemplateDirty = FALSE;
        }
    }

Cleanup:
    ReleaseObject(pTemplate);

    return hr;
}

HRESULT CControl::ApplyTemplate(CControlTemplate* pTemplate)
{
    HRESULT hr = S_OK;

    IFCPTR(pTemplate);

Cleanup:
    return hr;
}

HRESULT CControl::GetEffectiveTemplate(CControlTemplate** ppTemplate)
{
    HRESULT hr = S_OK;

    IFCPTR(ppTemplate);

    IFC(m_Template.GetTypedEffectiveValue(GetProviders(), ppTemplate));

Cleanup:
    return hr;
}

HRESULT CControl::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &TemplateProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CControl::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CControl::TemplateProperty)
    {
        *ppLayeredValue = &m_Template;
    }
    else
    {
        hr = CUIElement::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}