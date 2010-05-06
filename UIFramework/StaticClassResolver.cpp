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

template< typename FromType >
class StaticClassFactory
{
    public:
        static HRESULT Create(CPropertyObject** ppObject)
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

#define STATIC_CLASS_INFO(classtype) ObjectTypeTraits< classtype >::Type, &StaticClassFactory< classtype >::Create, classtype::CreatePropertyInformation

CStaticResolvedClass Classes[] =
{
    CStaticResolvedClass(L"Border", STATIC_CLASS_INFO(CBorder)),
    CStaticResolvedClass(L"Canvas", STATIC_CLASS_INFO(CCanvas)),
    //CStaticResolvedClass(L"Grid", STATIC_CLASS_INFO(CGrid)),
    CStaticResolvedClass(L"Image", STATIC_CLASS_INFO(CImage)),
    CStaticResolvedClass(L"StackPanel", STATIC_CLASS_INFO(CStackPanel)),
    CStaticResolvedClass(L"DockPanel", STATIC_CLASS_INFO(CDockPanel)),
    CStaticResolvedClass(L"TextBlock", STATIC_CLASS_INFO(CTextBlock)),
    CStaticResolvedClass(L"ImageBrush", STATIC_CLASS_INFO(CImageBrush)),
    CStaticResolvedClass(L"SolidColorBrush", STATIC_CLASS_INFO(CSolidColorBrush)),
    CStaticResolvedClass(L"Style", STATIC_CLASS_INFO(CStyle)),
    CStaticResolvedClass(L"Setter", STATIC_CLASS_INFO(CSetter)),
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
    IFCPTR(pImplicitClassProperties);

    pPropertyStart = wcschr(pPropertyName, L'.');
    
    if(pPropertyStart == NULL)
    {
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


CStaticResolvedClass::CStaticResolvedClass(const WCHAR* pTypeName, TypeIndex::Value ClassType, CreateTypeFunc CreateFunc, GetPropertyInformationFunc GetPropertiesFunc) : m_Name(pTypeName),
                                                                                                                                                                          m_ClassType(ClassType),
                                                                                                                                                                          m_CreateFunc(CreateFunc),
                                                                                                                                                                          m_GetPropertiesFunc(GetPropertiesFunc),
                                                                                                                                                                          m_CachedProperties(NULL)
{
}

CStaticResolvedClass::~CStaticResolvedClass()
{
    ReleaseObject(m_CachedProperties);
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

HRESULT CStaticResolvedClass::CreateInstance(CPropertyObject** ppObject)
{
    HRESULT hr = S_OK;

    IFCPTR(m_CreateFunc);

    IFC(m_CreateFunc(ppObject));

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