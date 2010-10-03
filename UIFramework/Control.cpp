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
DEFINE_GET_DEFAULT( BorderThickness, CFloatValue, 0 );
DEFINE_GET_DEFAULT( HorizontalContentAlignment, CHorizontalAlignmentValue, HorizontalAlignment::Stretch );
DEFINE_GET_DEFAULT( VerticalContentAlignment, CVerticalAlignmentValue, VerticalAlignment::Stretch );

//
// Properties
//
CStaticProperty CControl::TemplateProperty( L"Template", TypeIndex::ControlTemplate, StaticPropertyFlags::None, &GET_DEFAULT( Template ), &INSTANCE_CHANGE_CALLBACK( CControl, OnTemplateChanged ) );
CStaticProperty CControl::BackgroundProperty( L"Background", TypeIndex::Brush, StaticPropertyFlags::None, &GET_DEFAULT( Background ) );
CStaticProperty CControl::BorderBrushProperty( L"BorderBrush", TypeIndex::Brush, StaticPropertyFlags::None, &GET_DEFAULT( BorderBrush ) );
CStaticProperty CControl::BorderThicknessProperty( L"BorderThickness", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT(BorderThickness) );
CStaticProperty CControl::HorizontalContentAlignmentProperty( L"HorizontalContentAlignment", TypeIndex::HorizontalAlignment, StaticPropertyFlags::None, &GET_DEFAULT(HorizontalContentAlignment) );
CStaticProperty CControl::VerticalContentAlignmentProperty( L"VerticalContentAlignment", TypeIndex::VerticalAlignment, StaticPropertyFlags::None, &GET_DEFAULT(VerticalContentAlignment) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CControl, OnTemplateChanged );

CControl::CControl() : m_Template(this, &CControl::TemplateProperty),
                       m_Background(this, &CControl::BackgroundProperty),
                       m_BorderBrush(this, &CControl::BorderBrushProperty),
                       m_BorderThickness(this, &CControl::BorderThicknessProperty),
                       m_HorizontalContentAlignment(this, &CControl::HorizontalContentAlignmentProperty),
                       m_VerticalContentAlignment(this, &CControl::VerticalContentAlignmentProperty),
                       m_TemplateDirty(TRUE),
                       m_TemplateChild(NULL),
                       m_TemplateNamescope(NULL)
{
}

CControl::~CControl()
{
    ReleaseObject(m_TemplateChild);
    ReleaseObject(m_TemplateNamescope);
}

HRESULT CControl::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;
    CBoolValue* pFocusable = NULL;

    IFC(CFrameworkElement::Initialize(pProviders));

    IFC(CBoolValue::Create(TRUE, &pFocusable));

    IFC(SetValue(&CUIElement::FocusableProperty, pFocusable));

Cleanup:
    ReleaseObject(pFocusable);

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

    IFC(RevokeTemplate());

    m_TemplateDirty = TRUE;

    IFC(CFrameworkElement::OnDetach(Context));

Cleanup:
    return hr;
}

HRESULT CControl::OnTemplateChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    m_TemplateDirty = TRUE;

    IFC(EnsureTemplate());

Cleanup:
    return hr;
}

HRESULT CControl::GetTemplateChild(const WCHAR* pName, CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;

    IFCPTR(pName);
    IFCPTR(ppObject);

    IFC(EnsureTemplate());

    if(m_TemplateNamescope != NULL)
    {
        IFC(m_TemplateNamescope->FindName(pName, ppObject));
    }
    else
    {
        *ppObject = NULL;
    }

Cleanup:
    return hr;
}

HRESULT CControl::RevokeTemplate()
{
    HRESULT hr = S_OK;

    if(m_TemplateChild != NULL)
    {
        IFC(PreTemplateRevoked());

        IFC(RemoveLogicalChild(m_TemplateChild));

        ReleaseObject(m_TemplateChild);

        ReleaseObject(m_TemplateNamescope);
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
            m_TemplateDirty = FALSE;

            IFC(RevokeTemplate());

            IFC(GetEffectiveTemplate(&pTemplate));

            if(pTemplate)
            {
                IFC(ApplyTemplate(pTemplate));
            }

            IFC(PostTemplateApplied());
        }
    }

Cleanup:
    ReleaseObject(pTemplate);

    return hr;
}

HRESULT CControl::PostTemplateApplied()
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CControl::PreTemplateRevoked()
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CControl::ApplyTemplate(CControlTemplate* pTemplate)
{
    HRESULT hr = S_OK;
    CObjectWithType* pRootObject = NULL;

    IFCPTR(pTemplate);

    IFC(CNamescope::Create(&m_TemplateNamescope));

    IFC(pTemplate->LoadContent(m_TemplateNamescope, &pRootObject));

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

    IFC(m_Template.GetTypedEffectiveValue(ppTemplate));

Cleanup:
    return hr;
}

HRESULT CControl::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &TemplateProperty,
        &BackgroundProperty,
        &BorderBrushProperty,
        &BorderThicknessProperty,
        &HorizontalContentAlignmentProperty,
        &VerticalContentAlignmentProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

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
    else if(pProperty == &CControl::BorderThicknessProperty)
    {
        *ppLayeredValue = &m_BorderThickness;
    }
    else if(pProperty == &CControl::HorizontalContentAlignmentProperty)
    {
        *ppLayeredValue = &m_HorizontalContentAlignment;
    }
    else if(pProperty == &CControl::VerticalContentAlignmentProperty)
    {
        *ppLayeredValue = &m_VerticalContentAlignment;
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
    SizeF ChildSizeDesired;
    CUIElement* pChild = NULL;

    IFC(GetTemplateChild(&pChild));

    if(pChild != NULL)
    {
        IFC(pChild->Measure(AvailableSize));

        ChildSizeDesired = pChild->GetDesiredSize();
    }

    DesiredSize.width = ChildSizeDesired.width;
    DesiredSize.height = ChildSizeDesired.height;

Cleanup:
    ReleaseObject(pChild);

    return hr;
}

HRESULT CControl::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    CUIElement* pChild = NULL;

    IFC(GetTemplateChild(&pChild));

    if(pChild != NULL)
    {
        IFC(pChild->Arrange(MakeRect(AvailableSize)));

        UsedSize = pChild->GetFinalSize();
    }
    else
    {
        UsedSize.width = 0;
        UsedSize.height = 0;
    }
      
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

//
// CControl
//
extern "C" __declspec(dllexport)
TypeIndex::Value CControl_TypeIndex()
{
    return TypeIndex::Control;
}

extern "C" __declspec(dllexport)
CFrameworkElement* CControl_CastTo_CFrameworkElement(CControl* pControl)
{
    return pControl;
}

extern "C" __declspec(dllexport)
CControl* CObjectWithType_CastTo_CControl(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::Control)) ? (CControl*)pObject : NULL;
}