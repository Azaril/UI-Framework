#include "ParserUtilities.h"
#include "RichPropertyNodeCallback.h"
#include "ContentPropertyNodeCallback.h"

BOOL IsAttribute(const WCHAR* pText)
{
    if(pText != NULL)
    {
        return (wcschr(pText, L'.') != NULL);
    }

    return FALSE;
}

HRESULT ElementStartToParserCallback(CParseContext* pContext, CXMLElementStart* pStart, CElementNodeCallback** ppCallback)
{
    HRESULT hr = S_OK;

    IFCPTR(pContext);
    IFCPTR(pStart);
    IFCPTR(ppCallback);

    IFC(ElementStartToParserCallback(pContext, NULL, pStart, ppCallback));

Cleanup:
    return hr;
}

HRESULT ElementStartToParserCallback(CParseContext* pContext, CPropertyObject* pParent, CXMLElementStart* pStart, CElementNodeCallback** ppCallback)
{
    HRESULT hr = S_OK;
    CElementNodeCallback* pElementCallback = NULL;

    IFCPTR(pContext);
    IFCPTR(pStart);
    IFCPTR(ppCallback);

    IFC(CElementNodeCallback::Create(pContext, pParent, pStart, &pElementCallback));

    *ppCallback = pElementCallback;
    pElementCallback = NULL;

Cleanup:
    ReleaseObject(pElementCallback);

    return hr;
}

HRESULT ElementStartToParserCallback(CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLElementStart* pStart, CPropertyNodeCallback** ppCallback)
{
    HRESULT hr = S_OK;
    CRichPropertyNodeCallback* pRichPropertyCallback = NULL;
    CContentPropertyNodeCallback* pContentPropertyCallback = NULL;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;

    IFCPTR(pStart);
    IFCPTR(pParent);
    IFCPTR(pPropertyInformation);
    IFCPTR(ppCallback);

    IFC(pStart->GetName(&pElementName, &ElementNameLength));

    if(IsAttribute(pElementName))
    {
        IFC(CRichPropertyNodeCallback::Create(pContext, pParent, pStart, &pRichPropertyCallback));

        *ppCallback = pRichPropertyCallback;
        pRichPropertyCallback = NULL;
    }
    else
    {
        IFC(CContentPropertyNodeCallback::Create(pContext, pParent, pPropertyInformation, pStart, &pContentPropertyCallback));

        *ppCallback = pContentPropertyCallback;
        pContentPropertyCallback = NULL;
    }

Cleanup:
    ReleaseObject(pRichPropertyCallback);
    ReleaseObject(pContentPropertyCallback);

    return hr;
}

HRESULT TextToParserCallback(CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLText* pText, CPropertyNodeCallback** ppCallback)
{
    HRESULT hr = S_OK;
    CContentPropertyNodeCallback* pContentPropertyCallback = NULL;

    IFCPTR(pContext);
    IFCPTR(pParent);
    IFCPTR(pPropertyInformation);
    IFCPTR(pText);
    IFCPTR(ppCallback);

    IFC(CContentPropertyNodeCallback::Create(pContext, pParent, pPropertyInformation, pText, &pContentPropertyCallback));

    *ppCallback = pContentPropertyCallback;
    pContentPropertyCallback = NULL;

Cleanup:
    ReleaseObject(pContentPropertyCallback);

    return hr;
}

HRESULT AssignProperty(CPropertyObject* pElement, CProperty* pProperty, CObjectWithType* pValue, CTypeConverter* pTypeConverter)
{
    HRESULT hr = S_OK;
    CObjectWithType* pConvertedType = NULL;
    CObjectWithType* pCollectionObject = NULL;
    CObjectCollection* pCollection = NULL;

    IFCPTR(pElement);
    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty->GetType() == pValue->GetType() || pValue->IsTypeOf(pProperty->GetType()))
    {
        pConvertedType = pValue;
        AddRefObject(pConvertedType);
    }
    else
    {
        IFCPTR(pTypeConverter);

        IFC(pTypeConverter->Convert(pValue, pProperty->GetType(), &pConvertedType));
    }

    if(pProperty->IsCollection())
    {
        IFC(pElement->GetValue(pProperty, &pCollectionObject));

        IFCPTR(pCollectionObject);

        pCollection = (CObjectCollection*)pCollectionObject;

        IFC(pCollection->AddObject(pConvertedType));
    }
    else
    {
        IFC(pElement->SetValue(pProperty, pConvertedType));
    }

Cleanup:
    ReleaseObject(pConvertedType);
    ReleaseObject(pCollectionObject);

    return hr;
}