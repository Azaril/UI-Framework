#include "Control.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Template );
DEFINE_GET_DEFAULT_NULL( Background );
DEFINE_GET_DEFAULT_NULL( BorderBrush );

//
// Properties
//
CStaticProperty CControl::TemplateProperty( L"Template", TypeIndex::ControlTemplate, StaticPropertyFlags::None, &GET_DEFAULT( Template ), &INSTANCE_CHANGE_CALLBACK( CControl, OnTemplateChanged ) );
CStaticProperty CControl::BackgroundProperty( L"Background", TypeIndex::Brush, StaticPropertyFlags::None, &GET_DEFAULT( Background ) );
CStaticProperty CControl::BorderBrushProperty( L"BorderBrush", TypeIndex::Brush, StaticPropertyFlags::None, &GET_DEFAULT( BorderBrush ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CControl, OnTemplateChanged );

CControl::CControl() : m_Template(this, &CControl::TemplateProperty),
                       m_Background(this, &CControl::BackgroundProperty),
                       m_BorderBrush(this, &CControl::BorderBrushProperty),
                       m_TemplateDirty(TRUE),
                       m_TemplateChild(NULL)
{
}

CControl::~CControl()
{
    ReleaseObject(m_TemplateChild);
}

HRESULT CControl::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize(pProviders));

Cleanup:
    return hr;
}

CUIElement* CControl::GetTemplateParentForChildren()
{
    return this;
}

HRESULT CControl::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::OnAttach(Context));

    IFC(EnsureTemplate());

Cleanup:
    return hr;
}

HRESULT CControl::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::OnDetach(Context));

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
    CObjectWithType* pRootObject = NULL;

    IFCPTR(pTemplate);

    IFC(pTemplate->LoadContent(&pRootObject));

    IFCPTR(pRootObject);

    IFCEXPECT(pRootObject->IsTypeOf(TypeIndex::UIElement));

    m_TemplateChild = (CUIElement*)pRootObject;
    pRootObject = NULL;

    IFC(AddLogicalChild(m_TemplateChild));

Cleanup:
    ReleaseObject(pRootObject);

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
        &TemplateProperty,
        &BackgroundProperty,
        &BorderBrushProperty
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
    else if(pProperty == &CControl::BackgroundProperty)
    {
        *ppLayeredValue = &m_Background;
    }
    else if(pProperty == &CControl::BorderBrushProperty)
    {
        *ppLayeredValue = &m_BorderBrush;
    }
    else
    {
        hr = CFrameworkElement::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CControl::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF InternalSize = { 0 };
    SizeF BaseSize = { 0 };
    SizeF ChildSizeDesired = { 0 };
    CUIElement* pChild = NULL;

    IFC(CFrameworkElement::MeasureInternal(AvailableSize, BaseSize));

    IFC(GetTemplateChild(&pChild));

    if(pChild != NULL)
    {
        IFC(pChild->Measure(AvailableSize));

        ChildSizeDesired = pChild->GetDesiredSize();
    }

    DesiredSize.width = max(ChildSizeDesired.width, BaseSize.width);
    DesiredSize.height = max(ChildSizeDesired.height, BaseSize.height);

Cleanup:
    ReleaseObject(pChild);

    return hr;
}

HRESULT CControl::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;
    CUIElement* pChild = NULL;

    IFC(GetTemplateChild(&pChild));

    if(pChild != NULL)
    {
        IFC(pChild->Arrange(Size));
    }

    IFC(CFrameworkElement::ArrangeInternal(Size));
      
Cleanup:
    ReleaseObject(pChild);

    return hr;
}

HRESULT CControl::GetTemplateChild(CUIElement** ppChild)
{
    HRESULT hr = S_OK;

    IFCPTR(ppChild);

    *ppChild = m_TemplateChild;
    AddRefObject(m_TemplateChild);

Cleanup:
    return hr;
}