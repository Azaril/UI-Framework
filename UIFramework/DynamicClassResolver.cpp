#include "DynamicClassResolver.h"
#include "Border.h"
#include "Canvas.h"
#include "Grid.h"
#include "Image.h"
#include "StackPanel.h"
#include "TextBlock.h"
#include "SolidColorBrush.h"
#include "DockPanel.h"
#include "Style.h"
#include "Setter.h"
#include "DynamicResource.h"
#include "GradientStop.h"
#include "LinearGradientBrush.h"
#include "EventTrigger.h"
#include "ControlTemplate.h"
#include "Button.h"
#include "TemplateBinding.h"
#include "ContentPresenter.h"
#include "TypeMarkupExtension.h"
#include "TextBox.h"
#include "ScrollViewer.h"
#include "ScrollContentPresenter.h"
#include "RoutedCommand.h"
#include "ScrollBar.h"

template< typename FromType >
class StaticClassFactory
{
    public:
        static HRESULT Create(CProviders* pProviders, CPropertyObject** ppObject)
        {
            HRESULT hr = S_OK;
            FromType* pNewObject = NULL;

            IFCPTR(ppObject);

            IFC(FromType::Create(pProviders, &pNewObject));

            *ppObject = pNewObject;
            pNewObject = NULL;

        Cleanup:
            ReleaseObject(pNewObject);

            return hr;
        }
};

//    CStaticResolvedClass(L"UIElement", STATIC_CLASS_INFO_NOT_CREATABLE_WITH_EVENTS(CUIElement)),
//    CStaticResolvedClass(L"Border", STATIC_CLASS_INFO_WITH_EVENTS(CBorder)),
//    CStaticResolvedClass(L"Canvas", STATIC_CLASS_INFO_WITH_EVENTS(CCanvas)),
//    //CStaticResolvedClass(L"Grid", STATIC_CLASS_INFO_WITH_EVENTS(CGrid)),
//    CStaticResolvedClass(L"Image", STATIC_CLASS_INFO_WITH_EVENTS(CImage)),
//    CStaticResolvedClass(L"StackPanel", STATIC_CLASS_INFO_WITH_EVENTS(CStackPanel)),
//    CStaticResolvedClass(L"DockPanel", STATIC_CLASS_INFO_WITH_EVENTS(CDockPanel)),
//    CStaticResolvedClass(L"TextBlock", STATIC_CLASS_INFO_WITH_EVENTS(CTextBlock)),
//    CStaticResolvedClass(L"ContentPresenter", STATIC_CLASS_INFO_WITH_EVENTS(CContentPresenter)),
//    CStaticResolvedClass(L"ScrollContentPresenter", STATIC_CLASS_INFO_WITH_EVENTS(CScrollContentPresenter)),
//    CStaticResolvedClass(L"ScrollViewer", STATIC_CLASS_INFO_WITH_EVENTS(CScrollViewer)),
//    CStaticResolvedClass(L"TextBox", STATIC_CLASS_INFO_WITH_EVENTS(CTextBox)),
//    CStaticResolvedClass(L"Button", STATIC_CLASS_INFO_WITH_EVENTS(CButton)),
//    CStaticResolvedClass(L"ScrollBar", STATIC_CLASS_INFO_NOT_CREATABLE_WITH_EVENTS_AND_COMMANDS(CScrollBar)),
//    CStaticResolvedClass(L"ImageBrush", STATIC_CLASS_INFO(CImageBrush)),
//    CStaticResolvedClass(L"SolidColorBrush", STATIC_CLASS_INFO(CSolidColorBrush)),
//    CStaticResolvedClass(L"Style", STATIC_CLASS_INFO(CStyle)),
//    CStaticResolvedClass(L"Setter", STATIC_CLASS_INFO(CSetter)),
//    CStaticResolvedClass(L"DynamicResource", STATIC_CLASS_INFO(CDynamicResource)),
//    CStaticResolvedClass(L"LinearGradientBrush", STATIC_CLASS_INFO(CLinearGradientBrush)),
//    CStaticResolvedClass(L"GradientStop", STATIC_CLASS_INFO(CGradientStop)),
//    CStaticResolvedClass(L"EventTrigger", STATIC_CLASS_INFO(CEventTrigger)),
//    CStaticResolvedClass(L"ControlTemplate", STATIC_CLASS_INFO(CControlTemplate)),
//    CStaticResolvedClass(L"TemplateBinding", STATIC_CLASS_INFO(CTemplateBinding)),
//    CStaticResolvedClass(L"Type", STATIC_CLASS_INFO(CTypeMarkupExtension)),
//
//    CStaticResolvedClass(L"RoutedCommand", 
//        ObjectTypeTraits< CRoutedCommand >::Type, 
//        NULL, 
//        NULL,
//        CRoutedCommand::CreateEventInformation)

//#define STATIC_CLASS_INFO(classtype) ObjectTypeTraits< classtype >::Type, &StaticClassFactory< classtype >::Create, classtype::CreatePropertyInformation
//#define STATIC_CLASS_INFO_NOT_CREATABLE_WITH_EVENTS(classtype) ObjectTypeTraits< classtype >::Type, NULL, classtype::CreatePropertyInformation, classtype::CreateEventInformation
//#define STATIC_CLASS_INFO_NOT_CREATABLE_WITH_COMMANDS(classtype) ObjectTypeTraits< classtype >::Type, NULL, NULL, NULL, classtype::CreateCommandInformation
//#define STATIC_CLASS_INFO_NOT_CREATABLE_WITH_EVENTS_AND_COMMANDS(classtype) ObjectTypeTraits< classtype >::Type, NULL, classtype::CreatePropertyInformation, classtype::CreateEventInformation, classtype::CreateCommandInformation
//#define STATIC_CLASS_INFO_WITH_EVENTS(classtype) ObjectTypeTraits< classtype >::Type, &StaticClassFactory< classtype >::Create, classtype::CreatePropertyInformation, classtype::CreateEventInformation, classtype::CreateCommandInformation

ClassInformation Classes[] =
{
    ClassInformation(
        L"UIElement",
        TypeIndex::UIElement,
        NULL,
        &CUIElement::CreatePropertyInformation,
        &CUIElement::CreateEventInformation,
        &CUIElement::CreateCommandInformation),

    ClassInformation(
        L"Border",
        TypeIndex::Border,
        &StaticClassFactory< CBorder >::Create,
        &CBorder::CreatePropertyInformation,
        &CBorder::CreateEventInformation,
        &CBorder::CreateCommandInformation),

    ClassInformation(
        L"Canvas",
        TypeIndex::Canvas,
        &StaticClassFactory< CCanvas >::Create,
        &CCanvas::CreatePropertyInformation,
        &CCanvas::CreateEventInformation,
        &CCanvas::CreateCommandInformation),

    //ClassInformation(
    //    L"Grid",
    //    TypeIndex::Grid,
    //    &StaticClassFactory< CGrid >::Create,
    //    &CGrid::CreatePropertyInformation,
    //    &CGrid::CreateEventInformation,
    //    &CGrid::CreateCommandInformation),

    ClassInformation(
        L"Image",
        TypeIndex::Image,
        &StaticClassFactory< CImage >::Create,
        &CImage::CreatePropertyInformation,
        &CImage::CreateEventInformation,
        &CImage::CreateCommandInformation),

    ClassInformation(
        L"StackPanel",
        TypeIndex::StackPanel,
        &StaticClassFactory< CStackPanel >::Create,
        &CStackPanel::CreatePropertyInformation,
        &CStackPanel::CreateEventInformation,
        &CStackPanel::CreateCommandInformation),

    ClassInformation(
        L"DockPanel",
        TypeIndex::DockPanel,
        &StaticClassFactory< CDockPanel >::Create,
        &CDockPanel::CreatePropertyInformation,
        &CDockPanel::CreateEventInformation,
        &CDockPanel::CreateCommandInformation),

    ClassInformation(
        L"TextBlock",
        TypeIndex::TextBlock,
        &StaticClassFactory< CTextBlock >::Create,
        &CTextBlock::CreatePropertyInformation,
        &CTextBlock::CreateEventInformation,
        &CTextBlock::CreateCommandInformation),

    ClassInformation(
        L"ContentPresenter",
        TypeIndex::ContentPresenter,
        &StaticClassFactory< CContentPresenter >::Create,
        &CContentPresenter::CreatePropertyInformation,
        &CContentPresenter::CreateEventInformation,
        &CContentPresenter::CreateCommandInformation),

    ClassInformation(
        L"ScrollContentPresenter",
        TypeIndex::ScrollContentPresenter,
        &StaticClassFactory< CScrollContentPresenter >::Create,
        &CScrollContentPresenter::CreatePropertyInformation,
        &CScrollContentPresenter::CreateEventInformation,
        &CScrollContentPresenter::CreateCommandInformation),

    ClassInformation(
        L"ScrollViewer",
        TypeIndex::ScrollViewer,
        &StaticClassFactory< CScrollViewer >::Create,
        &CScrollViewer::CreatePropertyInformation,
        &CScrollViewer::CreateEventInformation,
        &CScrollViewer::CreateCommandInformation),

    ClassInformation(
        L"TextBox",
        TypeIndex::TextBox,
        &StaticClassFactory< CTextBox >::Create,
        &CTextBox::CreatePropertyInformation,
        &CTextBox::CreateEventInformation,
        &CTextBox::CreateCommandInformation),

    ClassInformation(
        L"Button",
        TypeIndex::Button,
        &StaticClassFactory< CButton >::Create,
        &CButton::CreatePropertyInformation,
        &CButton::CreateEventInformation,
        &CButton::CreateCommandInformation),

    ClassInformation(
        L"ScrollBar",
        TypeIndex::ScrollBar,
        &StaticClassFactory< CScrollBar >::Create,
        &CScrollBar::CreatePropertyInformation,
        &CScrollBar::CreateEventInformation,
        &CScrollBar::CreateCommandInformation),

    ClassInformation(
        L"ImageBrush",
        TypeIndex::ImageBrush,
        &StaticClassFactory< CImageBrush >::Create,
        &CImageBrush::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"SolidColorBrush",
        TypeIndex::SolidColorBrush,
        &StaticClassFactory< CSolidColorBrush >::Create,
        &CSolidColorBrush::CreatePropertyInformation,
        NULL,
        NULL),

   ClassInformation(
        L"LinearGradientBrush",
        TypeIndex::LinearGradientBrush,
        &StaticClassFactory< CLinearGradientBrush >::Create,
        &CLinearGradientBrush::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"Style",
        TypeIndex::Style,
        &StaticClassFactory< CStyle >::Create,
        &CStyle::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"Setter",
        TypeIndex::Setter,
        &StaticClassFactory< CSetter >::Create,
        &CSetter::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"DynamicResource",
        TypeIndex::DynamicResource,
        &StaticClassFactory< CDynamicResource >::Create,
        &CDynamicResource::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"GradientStop",
        TypeIndex::GradientStop,
        &StaticClassFactory< CGradientStop >::Create,
        &CGradientStop::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"EventTrigger",
        TypeIndex::EventTrigger,
        &StaticClassFactory< CEventTrigger >::Create,
        &CEventTrigger::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"ControlTemplate",
        TypeIndex::ControlTemplate,
        &StaticClassFactory< CControlTemplate >::Create,
        &CControlTemplate::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"TemplateBinding",
        TypeIndex::TemplateBinding,
        &StaticClassFactory< CTemplateBinding >::Create,
        &CTemplateBinding::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"Type",
        TypeIndex::Type,
        &StaticClassFactory< CTypeMarkupExtension >::Create,
        &CTypeMarkupExtension::CreatePropertyInformation,
        NULL,
        NULL)
};

HRESULT LoadDefaultTypes(CDynamicClassResolver* pClassResolver)
{
    HRESULT hr = S_OK;

    IFCPTR(pClassResolver);

    for(UINT32 i = 0; i < ARRAYSIZE(Classes); i++)
    {
        IFC(pClassResolver->RegisterClass(Classes[i]));
    }

Cleanup:
    return hr;
}

CDynamicClassResolver::CDynamicClassResolver()
{
}

CDynamicClassResolver::~CDynamicClassResolver()
{
    for(ClassContainer::iterator It = m_Classes.begin(); It != m_Classes.end(); ++It)
    {
        (*It)->Release();
    }

    m_Classes.clear();
}

HRESULT CDynamicClassResolver::Initialize()
{
    HRESULT hr = S_OK;

    IFC(LoadDefaultTypes(this));

Cleanup:
    return hr;
}

HRESULT CDynamicClassResolver::RegisterClass(const ClassInformation& Info)
{
    HRESULT hr = S_OK;
    CDynamicResolvedClass* pClass = NULL;

    IFC(CDynamicResolvedClass::Create(Info, &pClass));

    m_Classes.push_back(pClass);

Cleanup:
    return hr;
}

HRESULT CDynamicClassResolver::ResolveType(TypeIndex::Value ClassType, CResolvedClass** ppResolvedClass)
{
    HRESULT hr = S_OK;

    IFCPTR(ppResolvedClass);

    for(ClassContainer::iterator It = m_Classes.begin(); It != m_Classes.end(); ++It)
    {
        CDynamicResolvedClass* pClass = (*It);

        if(pClass->GetType() == ClassType)
        {
            *ppResolvedClass = pClass;

            AddRefObject(pClass);

            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CDynamicClassResolver::ResolveType(const WCHAR* pTypeName, CResolvedClass** ppResolvedClass)
{
    HRESULT hr = S_OK;

    IFCPTR(pTypeName);
    IFCPTR(ppResolvedClass);

    for(ClassContainer::iterator It = m_Classes.begin(); It != m_Classes.end(); ++It)
    {
        CDynamicResolvedClass* pClass = (*It);

        if(wcscmp(pTypeName, pClass->GetName()) == 0)
        {
            *ppResolvedClass = pClass;

            AddRefObject(pClass);

            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CDynamicClassResolver::ResolveProperties(TypeIndex::Value ClassType, CPropertyInformation** ppProperties)
{
    HRESULT hr = S_OK;
    CResolvedClass* pResolvedClass = NULL;

    IFCPTR(ppProperties);

    IFC(ResolveType(ClassType, &pResolvedClass));

    IFC(pResolvedClass->GetPropertyInformation(ppProperties));

Cleanup:
    ReleaseObject(pResolvedClass);

    return hr;
}

HRESULT CDynamicClassResolver::ResolveProperties(const WCHAR* pTypeName, CPropertyInformation** ppProperties)
{
    HRESULT hr = S_OK;
    CResolvedClass* pResolvedClass = NULL;

    IFCPTR(pTypeName);
    IFCPTR(ppProperties);

    IFC(ResolveType(pTypeName, &pResolvedClass));

    IFC(pResolvedClass->GetPropertyInformation(ppProperties));

Cleanup:
    ReleaseObject(pResolvedClass);

    return hr;
}

HRESULT CDynamicClassResolver::ResolveProperty(const WCHAR* pPropertyName, TypeIndex::Value ImplicitClass, CProperty** ppProperty)
{
    HRESULT hr = S_OK;
    CResolvedClass* pImplicitClass = NULL;
    CPropertyInformation* pImplicitClassProperties = NULL;

    IFCPTR(pPropertyName);
    IFCPTR(ppProperty);

    IFC(ResolveType(ImplicitClass, &pImplicitClass));

    IFC(pImplicitClass->GetPropertyInformation(&pImplicitClassProperties));

    IFC(ResolveProperty(pPropertyName, pImplicitClassProperties, ppProperty));

Cleanup:
    ReleaseObject(pImplicitClassProperties);
    ReleaseObject(pImplicitClass);

    return hr;
}

HRESULT CDynamicClassResolver::ResolveProperty(const WCHAR* pPropertyName, CPropertyInformation* pImplicitClassProperties, CProperty** ppProperty)
{
    HRESULT hr = S_OK;
    WCHAR* pClassType = NULL;
    UINT32 ClassTypeLength = 0;
    CPropertyInformation* pResolvedClassProperties = NULL;
    const WCHAR* pPropertyStart = NULL;

    IFCPTR(pPropertyName);
    IFCPTR(ppProperty);

    pPropertyStart = wcschr(pPropertyName, L'.');
    
    if(pPropertyStart == NULL)
    {
        IFCPTR(pImplicitClassProperties);

        pPropertyStart = pPropertyName;

        pResolvedClassProperties = pImplicitClassProperties;
        AddRefObject(pResolvedClassProperties);
    }
    else
    {
        ClassTypeLength = (pPropertyStart - pPropertyName);

        pClassType = new WCHAR[ClassTypeLength + 1];
        IFCOOM(pClassType);

        wcsncpy(pClassType, pPropertyName, ClassTypeLength);
        pClassType[ClassTypeLength] = L'\0';

        IFC(ResolveProperties(pClassType, &pResolvedClassProperties));

        ++pPropertyStart;
    }

    IFC(pResolvedClassProperties->GetProperty(pPropertyStart, ppProperty));

Cleanup:
    ReleaseObject(pResolvedClassProperties);
    delete [] pClassType;

    return hr;
}

HRESULT CDynamicClassResolver::ResolveEvent(const WCHAR* pEventName, TypeIndex::Value ImplicitClass, CRoutedEvent** ppRoutedEvent)
{
    HRESULT hr = S_OK;
    CResolvedClass* pImplicitClass = NULL;
    CEventInformation* pImplicitClassEvents = NULL;

    IFCPTR(pEventName);
    IFCPTR(ppRoutedEvent);

    IFC(ResolveType(ImplicitClass, &pImplicitClass));

    if(FAILED((pImplicitClass->GetEventInformation(&pImplicitClassEvents))))
    {
        pImplicitClassEvents = NULL;
    }

    IFC(ResolveEvent(pEventName, pImplicitClassEvents, ppRoutedEvent));

Cleanup:
    ReleaseObject(pImplicitClassEvents);
    ReleaseObject(pImplicitClass);

    return hr;
}

HRESULT CDynamicClassResolver::ResolveEvent(const WCHAR* pEventName, CEventInformation* pImplicitClassEvents, CRoutedEvent** ppRoutedEvent)
{
    HRESULT hr = S_OK;
    WCHAR* pClassType = NULL;
    UINT32 ClassTypeLength = 0;
    CEventInformation* pResolvedClassEvents = NULL;
    const WCHAR* pEventStart = NULL;

    IFCPTR(pEventName);
    IFCPTR(ppRoutedEvent);

    pEventStart = wcschr(pEventName, L'.');
    
    if(pEventStart == NULL)
    {
        pEventStart = pEventName;

        pResolvedClassEvents = pImplicitClassEvents;
        AddRefObject(pImplicitClassEvents);
    }
    else
    {
        ClassTypeLength = (pEventStart - pEventName);

        pClassType = new WCHAR[ClassTypeLength + 1];
        IFCOOM(pClassType);

        wcsncpy(pClassType, pEventName, ClassTypeLength);
        pClassType[ClassTypeLength] = L'\0';

        IFC(ResolveEvents(pClassType, &pResolvedClassEvents));

        ++pEventStart;
    }

    IFCPTR(pResolvedClassEvents);

    IFC(pResolvedClassEvents->GetEvent(pEventStart, ppRoutedEvent));

Cleanup:
    ReleaseObject(pResolvedClassEvents);
    delete [] pClassType;

    return hr;
}

HRESULT CDynamicClassResolver::ResolveEvents(TypeIndex::Value ClassType, CEventInformation** ppEvents)
{
    HRESULT hr = S_OK;
    CResolvedClass* pResolvedClass = NULL;

    IFCPTR(ppEvents);

    IFC(ResolveType(ClassType, &pResolvedClass));

    IFC(pResolvedClass->GetEventInformation(ppEvents));

Cleanup:
    ReleaseObject(pResolvedClass);

    return hr;
}

HRESULT CDynamicClassResolver::ResolveEvents(const WCHAR* pTypeName, CEventInformation** ppEvents)
{
    HRESULT hr = S_OK;
    CResolvedClass* pResolvedClass = NULL;

    IFCPTR(pTypeName);
    IFCPTR(ppEvents);

    IFC(ResolveType(pTypeName, &pResolvedClass));

    IFC(pResolvedClass->GetEventInformation(ppEvents));

Cleanup:
    ReleaseObject(pResolvedClass);

    return hr;
}

HRESULT CDynamicClassResolver::ResolveCommand(const WCHAR* pCommandName, TypeIndex::Value ImplicitClass, CCommand** ppCommand)
{
    HRESULT hr = S_OK;
    CResolvedClass* pImplicitClass = NULL;
    CCommandInformation* pImplicitClassCommands = NULL;

    IFCPTR(pCommandName);
    IFCPTR(ppCommand);

    IFC(ResolveType(ImplicitClass, &pImplicitClass));

    if(FAILED((pImplicitClass->GetCommandInformation(&pImplicitClassCommands))))
    {
        pImplicitClassCommands = NULL;
    }

    IFC(ResolveCommand(pCommandName, pImplicitClassCommands, ppCommand));

Cleanup:
    ReleaseObject(pImplicitClassCommands);
    ReleaseObject(pImplicitClass);

    return hr;
}

HRESULT CDynamicClassResolver::ResolveCommand(const WCHAR* pCommandName, CCommandInformation* pImplicitClassCommands, CCommand** ppCommand)
{
    HRESULT hr = S_OK;
    WCHAR* pClassType = NULL;
    UINT32 ClassTypeLength = 0;
    CCommandInformation* pResolvedClassCommands = NULL;
    const WCHAR* pCommandStart = NULL;

    IFCPTR(pCommandName);
    IFCPTR(ppCommand);

    pCommandStart = wcschr(pCommandName, L'.');
    
    if(pCommandStart == NULL)
    {
        pCommandStart = pCommandName;

        pResolvedClassCommands = pImplicitClassCommands;
        AddRefObject(pImplicitClassCommands);
    }
    else
    {
        ClassTypeLength = (pCommandStart - pCommandName);

        pClassType = new WCHAR[ClassTypeLength + 1];
        IFCOOM(pClassType);

        wcsncpy(pClassType, pCommandName, ClassTypeLength);
        pClassType[ClassTypeLength] = L'\0';

        IFC(ResolveCommands(pClassType, &pResolvedClassCommands));

        ++pCommandStart;
    }

    IFCPTR(pResolvedClassCommands);

    IFC(pResolvedClassCommands->GetCommand(pCommandStart, ppCommand));

Cleanup:
    ReleaseObject(pResolvedClassCommands);
    delete [] pClassType;

    return hr;
}

HRESULT CDynamicClassResolver::ResolveCommands(TypeIndex::Value ClassType, CCommandInformation** ppCommands)
{
    HRESULT hr = S_OK;
    CResolvedClass* pResolvedClass = NULL;

    IFCPTR(ppCommands);

    IFC(ResolveType(ClassType, &pResolvedClass));

    IFC(pResolvedClass->GetCommandInformation(ppCommands));

Cleanup:
    ReleaseObject(pResolvedClass);

    return hr;
}

HRESULT CDynamicClassResolver::ResolveCommands(const WCHAR* pTypeName, CCommandInformation** ppCommands)
{
    HRESULT hr = S_OK;
    CResolvedClass* pResolvedClass = NULL;

    IFCPTR(pTypeName);
    IFCPTR(ppCommands);

    IFC(ResolveType(pTypeName, &pResolvedClass));

    IFC(pResolvedClass->GetCommandInformation(ppCommands));

Cleanup:
    ReleaseObject(pResolvedClass);

    return hr;
}