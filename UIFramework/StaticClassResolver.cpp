#include "StaticClassResolver.h"
#include "Border.h"
#include "Canvas.h"
#include "Grid.h"
#include "Image.h"
#include "StackPanel.h"
#include "TextBlock.h"

template< typename T >
class StaticClassFactory
{
    public:
        static HRESULT Create(CPropertyObject** ppObject)
        {
            HRESULT hr = S_OK;
            T* pElement = NULL;

            IFCPTR(ppObject);

            IFC(T::Create(&pElement));

            *ppObject = pElement;
            pElement = NULL;

        Cleanup:
            ReleaseObject(pElement);

            return hr;
        }
};

struct ClassInformation
{
    WCHAR* ClassName;
    ClassType TypeInformation;
};

ClassInformation Info[] =
{
    { L"Border", { &StaticClassFactory< CBorder >::Create } },
    { L"Canvas", { &StaticClassFactory< CCanvas >::Create } },
    //{ L"Grid", { &StaticClassFactory< CGrid >::Create } },
    { L"Image", { &StaticClassFactory< CImage >::Create } },
    { L"StackPanel", { &StaticClassFactory< CStackPanel >::Create } },
    { L"TextBlock", { &StaticClassFactory< CTextBlock >::Create } },
    { L"ImageBrush", { &StaticClassFactory< CImageBrush >::Create } }
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

HRESULT CStaticClassResolver::ResolveType(const WCHAR* pTypeName, ClassType* pClassType)
{
    HRESULT hr = S_OK;

    IFCPTR(pTypeName);
    IFCPTR(pClassType);

    for(UINT32 i = 0; i < ARRAYSIZE(Info); i++)
    {
        if(wcscmp(pTypeName, Info[i].ClassName) == 0)
        {
            *pClassType = Info[i].TypeInformation;
            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}