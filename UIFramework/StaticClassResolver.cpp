#include "StaticClassResolver.h"
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

#define STATIC_CLASS_INFO(classtype) ObjectTypeTraits< classtype >::Type, &StaticClassFactory< classtype >::Create, classtype::CreatePropertyInformation

CStaticResolvedClass Classes[] =
{
    //TODO: Update event information providers.
    CStaticResolvedClass(L"UIElement", ObjectTypeTraits< CUIElement >::Type, NULL, CUIElement::CreatePropertyInformation, CUIElement::CreateEventInformation),
    CStaticResolvedClass(L"Border", STATIC_CLASS_INFO(CBorder), &CUIElement::CreateEventInformation),
    CStaticResolvedClass(L"Canvas", STATIC_CLASS_INFO(CCanvas), &CUIElement::CreateEventInformation),
    //CStaticResolvedClass(L"Grid", STATIC_CLASS_INFO(CGrid), &CUIElement::CreateEventInformation),
    CStaticResolvedClass(L"Image", STATIC_CLASS_INFO(CImage), &CUIElement::CreateEventInformation),
    CStaticResolvedClass(L"StackPanel", STATIC_CLASS_INFO(CStackPanel), &CUIElement::CreateEventInformation),
    CStaticResolvedClass(L"DockPanel", STATIC_CLASS_INFO(CDockPanel), &CUIElement::CreateEventInformation),
    CStaticResolvedClass(L"TextBlock", STATIC_CLASS_INFO(CTextBlock), &CUIElement::CreateEventInformation),
    CStaticResolvedClass(L"ContentPresenter", STATIC_CLASS_INFO(CContentPresenter), &CUIElement::CreateEventInformation),
    CStaticResolvedClass(L"ImageBrush", STATIC_CLASS_INFO(CImageBrush)),
    CStaticResolvedClass(L"SolidColorBrush", STATIC_CLASS_INFO(CSolidColorBrush)),
    CStaticResolvedClass(L"Style", STATIC_CLASS_INFO(CStyle)),
    CStaticResolvedClass(L"Setter", STATIC_CLASS_INFO(CSetter)),
    CStaticResolvedClass(L"DynamicResource", STATIC_CLASS_INFO(CDynamicResource)),
    CStaticResolvedClass(L"LinearGradientBrush", STATIC_CLASS_INFO(CLinearGradientBrush)),
    CStaticResolvedClass(L"GradientStop", STATIC_CLASS_INFO(CGradientStop)),
    CStaticResolvedClass(L"EventTrigger", STATIC_CLASS_INFO(CEventTrigger)),
    CStaticResolvedClass(L"ControlTemplate", STATIC_CLASS_INFO(CControlTemplate)),
    CStaticResolvedClass(L"Button", STATIC_CLASS_INFO(CButton)),
    CStaticResolvedClass(L"TemplateBinding", STATIC_CLASS_INFO(CTemplateBinding)),
    CStaticResolvedClass(L"Type", STATIC_CLASS_INFO(CTypeMarkupExtension)),
};

CStaticClassResolver::CStaticClassResolver()
{
}

CStaticClassResolver::~CStaticClassResolver()
{
}

HRESULT CStaticClassResolver::Initialize()
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CStaticClassResolver::ResolveType(TypeIndex::Value ClassType, CResolvedClass** ppResolvedClass)
{
    HRESULT hr = S_OK;

    IFCPTR(ppResolvedClass);

    for(UINT32 i = 0; i < ARRAYSIZE(Classes); i++)
    {
        if(ClassType == Classes[i].GetType())
        {
            *ppResolvedClass = &Classes[i];
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CStaticClassResolver::ResolveType(const WCHAR* pTypeName, CResolvedClass** ppResolvedClass)
{
    HRESULT hr = S_OK;

    IFCPTR(pTypeName);
    IFCPTR(ppResolvedClass);

    for(UINT32 i = 0; i < ARRAYSIZE(Classes); i++)
    {
        if(wcscmp(pTypeName, Classes[i].GetName()) == 0)
        {
            *ppResolvedClass = &Classes[i];
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CStaticClassResolver::ResolveProperties(TypeIndex::Value ClassType, CPropertyInformation** ppProperties)
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

HRESULT CStaticClassResolver::ResolveProperties(const WCHAR* pTypeName, CPropertyInformation** ppProperties)
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

HRESULT CStaticClassResolver::ResolveProperty(const WCHAR* pPropertyName, TypeIndex::Value ImplicitClass, CProperty** ppProperty)
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

HRESULT CStaticClassResolver::ResolveProperty(const WCHAR* pPropertyName, CPropertyInformation* pImplicitClassProperties, CProperty** ppProperty)
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

        wcsncpy_s(pClassType, ClassTypeLength + 1, pPropertyName, ClassTypeLength);
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

HRESULT CStaticClassResolver::ResolveEvent(const WCHAR* pEventName, TypeIndex::Value ImplicitClass, CRoutedEvent** ppRoutedEvent)
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

HRESULT CStaticClassResolver::ResolveEvent(const WCHAR* pEventName, CEventInformation* pImplicitClassEvents, CRoutedEvent** ppRoutedEvent)
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

        wcsncpy_s(pClassType, ClassTypeLength + 1, pEventName, ClassTypeLength);
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

HRESULT CStaticClassResolver::ResolveEvents(TypeIndex::Value ClassType, CEventInformation** ppEvents)
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

HRESULT CStaticClassResolver::ResolveEvents(const WCHAR* pTypeName, CEventInformation** ppEvents)
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


CStaticResolvedClass::CStaticResolvedClass(const WCHAR* pTypeName, TypeIndex::Value ClassType, CreateTypeFunc CreateFunc, GetPropertyInformationFunc GetPropertiesFunc, GetEventInformationFunc GetEventsFunc) : m_Name(pTypeName),
                                                                                                                                                                                                                 m_ClassType(ClassType),
                                                                                                                                                                                                                 m_CreateFunc(CreateFunc),
                                                                                                                                                                                                                 m_GetPropertiesFunc(GetPropertiesFunc),
                                                                                                                                                                                                                 m_GetEventsFunc(GetEventsFunc),
                                                                                                                                                                                                                 m_CachedProperties(NULL),
                                                                                                                                                                                                                 m_CachedEvents(NULL)
{
}

CStaticResolvedClass::~CStaticResolvedClass()
{
    ReleaseObject(m_CachedProperties);
    ReleaseObject(m_CachedEvents);
}

INT32 CStaticResolvedClass::AddRef()
{
    return 1;
}

INT32 CStaticResolvedClass::Release()
{
    return 1;
}

const WCHAR* CStaticResolvedClass::GetName()
{
    return m_Name;
}

TypeIndex::Value CStaticResolvedClass::GetType()
{
    return m_ClassType;
}

HRESULT CStaticResolvedClass::CreateInstance(CProviders* pProviders, CPropertyObject** ppObject)
{
    HRESULT hr = S_OK;

    IFCPTR(m_CreateFunc);

    IFC(m_CreateFunc(pProviders, ppObject));

Cleanup:
    return hr;
}

HRESULT CStaticResolvedClass::GetPropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;

    if(m_CachedProperties == NULL)
    {
        IFCPTR(m_GetPropertiesFunc);

        IFC(m_GetPropertiesFunc(&m_CachedProperties));
    }

    *ppInformation = m_CachedProperties;
    AddRefObject(m_CachedProperties);

Cleanup:
    return hr;
}

HRESULT CStaticResolvedClass::GetEventInformation(CEventInformation** ppInformation)
{
    HRESULT hr = S_OK;

    if(m_CachedEvents == NULL)
    {
        IFCPTR(m_GetEventsFunc);

        IFC(m_GetEventsFunc(&m_CachedEvents));
    }

    *ppInformation = m_CachedEvents;
    AddRefObject(m_CachedEvents);

Cleanup:
    return hr;
}