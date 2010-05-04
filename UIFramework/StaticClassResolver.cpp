#include "StaticClassResolver.h"
#include "Border.h"
#include "Canvas.h"
#include "Grid.h"
#include "Image.h"
#include "StackPanel.h"
#include "TextBlock.h"
#include "SolidColorBrush.h"

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

CStaticResolvedClass Classes[] =
{
    CStaticResolvedClass(L"Border", &StaticClassFactory< CBorder >::Create, CBorder::CreatePropertyInformation),
    CStaticResolvedClass(L"Canvas", &StaticClassFactory< CCanvas >::Create, CCanvas::CreatePropertyInformation),
    //CStaticResolvedClass(L"Grid", { &StaticClassFactory< CGrid >::Create, CGrid::CreatePropertyInformation),
    CStaticResolvedClass(L"Image", &StaticClassFactory< CImage >::Create, CImage::CreatePropertyInformation),
    CStaticResolvedClass(L"StackPanel", &StaticClassFactory< CStackPanel >::Create, CStackPanel::CreatePropertyInformation),
    CStaticResolvedClass(L"TextBlock", &StaticClassFactory< CTextBlock >::Create, CTextBlock::CreatePropertyInformation),
    CStaticResolvedClass(L"ImageBrush", &StaticClassFactory< CImageBrush >::Create, CImageBrush::CreatePropertyInformation),
    CStaticResolvedClass(L"SolidColorBrush", &StaticClassFactory< CSolidColorBrush >::Create, CSolidColorBrush::CreatePropertyInformation),
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



CStaticResolvedClass::CStaticResolvedClass(const WCHAR* pTypeName, CreateTypeFunc CreateFunc, GetPropertyInformationFunc GetPropertiesFunc) : m_Name(pTypeName),
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