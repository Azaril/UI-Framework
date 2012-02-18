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
#include "Thumb.h"
#include "Track.h"
#include "RangeBase.h"
#include "BeginStoryboard.h"
#include "AnimationTimeline.h"
#include "FloatAnimation.h"
#include "Storyboard.h"

template< typename FromType >
class StaticClassFactory
{
    public:
        static HRESULT Create(
			__in CProviders* pProviders,
			__deref_out CPropertyObject** ppObject
			)
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

template< typename FromType >
class StaticClassFactoryNoProviders
{
    public:
        static HRESULT Create(
			__in CProviders* pProviders,
			__deref_out CPropertyObject** ppObject
			)
        {
            HRESULT hr = S_OK;
            FromType* pNewObject = NULL;

            IFCPTR(ppObject);

            IFC(FromType::Create(&pNewObject));

            *ppObject = pNewObject;
            pNewObject = NULL;

        Cleanup:
            ReleaseObject(pNewObject);

            return hr;
        }
};

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
        L"FrameworkElement",
        TypeIndex::FrameworkElement,
        NULL,
        &CFrameworkElement::CreatePropertyInformation,
        &CFrameworkElement::CreateEventInformation,
        &CFrameworkElement::CreateCommandInformation),

    ClassInformation(
        L"Control",
        TypeIndex::Control,
        NULL,
        &CControl::CreatePropertyInformation,
        &CControl::CreateEventInformation,
        &CControl::CreateCommandInformation),

    ClassInformation(
        L"ContentControl",
        TypeIndex::ContentControl,
        NULL,
        &CContentControl::CreatePropertyInformation,
        &CContentControl::CreateEventInformation,
        &CContentControl::CreateCommandInformation),

    ClassInformation(
        L"Decorator",
        TypeIndex::Decorator,
        NULL,
        &CDecorator::CreatePropertyInformation,
        &CDecorator::CreateEventInformation,
        &CDecorator::CreateCommandInformation),

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

    ClassInformation(
        L"Grid",
        TypeIndex::Grid,
        &StaticClassFactory< CGrid >::Create,
        &CGrid::CreatePropertyInformation,
        &CGrid::CreateEventInformation,
        &CGrid::CreateCommandInformation),

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
        L"ButtonBase",
        TypeIndex::ButtonBase,
        NULL,
        &CButtonBase::CreatePropertyInformation,
        &CButtonBase::CreateEventInformation,
        &CButtonBase::CreateCommandInformation),

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
        L"Thumb",
        TypeIndex::Thumb,
        &StaticClassFactory< CThumb >::Create,
        &CThumb::CreatePropertyInformation,
        &CThumb::CreateEventInformation,
        &CThumb::CreateCommandInformation),

    ClassInformation(
        L"RangeBase",
        TypeIndex::RangeBase,
        NULL,
        &CRangeBase::CreatePropertyInformation,
        &CRangeBase::CreateEventInformation,
        &CRangeBase::CreateCommandInformation),

    ClassInformation(
        L"Track",
        TypeIndex::Track,
        &StaticClassFactory< CTrack >::Create,
        &CTrack::CreatePropertyInformation,
        &CTrack::CreateEventInformation,
        &CTrack::CreateCommandInformation),

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
        L"SetterCollection",
        TypeIndex::SetterCollection,
        &StaticClassFactoryNoProviders< CSetterCollection >::Create,
        NULL,
        NULL,
        NULL),

    ClassInformation(
        L"TriggerCollection",
        TypeIndex::TriggerCollection,
        &StaticClassFactoryNoProviders< CTriggerCollection >::Create,
        NULL,
        NULL,
        NULL),

    ClassInformation(
        L"TriggerActionCollection",
        TypeIndex::TriggerActionCollection,
        &StaticClassFactoryNoProviders< CTriggerActionCollection >::Create,
        NULL,
        NULL,
        NULL),

    ClassInformation(
        L"BeginStoryboard",
        TypeIndex::BeginStoryboard,
        &StaticClassFactory< CBeginStoryboard >::Create,
        &CBeginStoryboard::CreatePropertyInformation,
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
        L"GradientStopCollection",
        TypeIndex::GradientStopCollection,
        &StaticClassFactoryNoProviders< CGradientStopCollection >::Create,
        NULL,
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
        L"TriggerCollection",
        TypeIndex::TriggerCollection,
        &StaticClassFactoryNoProviders< CTriggerCollection >::Create,
        NULL,
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
        NULL),

    ClassInformation(
        L"AnimationTimeline",
        TypeIndex::AnimationTimeline,
        NULL,
        &CAnimationTimeline::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"AnimationTimelineCollection",
        TypeIndex::AnimationTimelineCollection,
        &StaticClassFactoryNoProviders< CAnimationTimelineCollection >::Create,
        NULL,
        NULL,
        NULL),

    ClassInformation(
        L"FloatAnimation",
        TypeIndex::FloatAnimation,
        &StaticClassFactory< CFloatAnimation >::Create,
        &CFloatAnimation::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"Storyboard",
        TypeIndex::Storyboard,
        &StaticClassFactory< CStoryboard >::Create,
        &CStoryboard::CreatePropertyInformation,
        NULL,
        NULL),

    ClassInformation(
        L"ResourceDictionary",
        TypeIndex::ResourceDictionary,
        &StaticClassFactoryNoProviders< CResourceDictionary >::Create,
        NULL,
        NULL,
        NULL)
};

__checkReturn HRESULT LoadDefaultTypes(
	__in CDynamicClassResolver* pClassResolver
	)
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

CDynamicClassResolver::CDynamicClassResolver(
	)
{
}

CDynamicClassResolver::~CDynamicClassResolver(
	)
{
    for(ClassContainer::iterator It = m_Classes.begin(); It != m_Classes.end(); ++It)
    {
        (*It)->Release();
    }

    m_Classes.clear();
}

__checkReturn HRESULT 
CDynamicClassResolver::Initialize(
	)
{
    HRESULT hr = S_OK;

    IFC(LoadDefaultTypes(this));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CDynamicClassResolver::RegisterClass(
	const ClassInformation& Info
	)
{
    HRESULT hr = S_OK;
    CDynamicResolvedClass* pClass = NULL;

    IFC(CDynamicResolvedClass::Create(Info, &pClass));

    m_Classes.push_back(pClass);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CDynamicClassResolver::ResolveType(
	TypeIndex::Value ClassType, 
	__deref_out CResolvedClass** ppResolvedClass
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveType(
	__in_z const WCHAR* pTypeName, 
	__deref_out CResolvedClass** ppResolvedClass
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveProperties(
	TypeIndex::Value ClassType, 
	__deref_out CPropertyInformation** ppProperties
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveProperties(
	__in_z const WCHAR* pTypeName, 
	__deref_out CPropertyInformation** ppProperties
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveProperty(
	__in_z const WCHAR* pPropertyName,
	TypeIndex::Value ImplicitClass, 
	__deref_out CProperty** ppProperty
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveProperty(
	__in_z const WCHAR* pPropertyName, 
	__in_opt CPropertyInformation* pImplicitClassProperties,
	__deref_out CProperty** ppProperty
	)
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

__checkReturn HRESULT
CDynamicClassResolver::ResolveEvent(
	const WCHAR* pEventName, 
	TypeIndex::Value ImplicitClass,
	__deref_out CRoutedEvent** ppRoutedEvent
	)
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

__checkReturn HRESULT
CDynamicClassResolver::ResolveEvent(
	__in_z const WCHAR* pEventName, 
	__in_opt CEventInformation* pImplicitClassEvents, 
	__deref_out CRoutedEvent** ppRoutedEvent
	)
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

__checkReturn HRESULT
CDynamicClassResolver::ResolveEvents(
	TypeIndex::Value ClassType, 
	__deref_out CEventInformation** ppEvents
	)
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

__checkReturn HRESULT
CDynamicClassResolver::ResolveEvents(
	__in_z const WCHAR* pTypeName,
	__deref_out CEventInformation** ppEvents
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveCommand(
	__in_z const WCHAR* pCommandName, 
	TypeIndex::Value ImplicitClass,
	__deref_out CCommand** ppCommand
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveCommand(
	__in_z const WCHAR* pCommandName, 
	__in_opt CCommandInformation* pImplicitClassCommands, 
	__deref_out CCommand** ppCommand
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveCommands(
	TypeIndex::Value ClassType, 
	__deref_out CCommandInformation** ppCommands
	)
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

__checkReturn HRESULT 
CDynamicClassResolver::ResolveCommands(
	__in_z const WCHAR* pTypeName, 
	__deref_out CCommandInformation** ppCommands
	)
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