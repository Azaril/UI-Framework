#include "ParserUtilities.h"
#include "RichPropertyNodeCallback.h"
#include "ContentPropertyNodeCallback.h"
#include "BasicTypes.h"

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

namespace MarkupExtensionParseState
{
    enum Value
    {
        Start,
        ParseMarkupType,
        CreateMarkupType,
        BeginFindProperty,
        FindProperty,
        ParseProperty,
        ParsePropertyValue,
        BeginParsePropertyValue,
        Complete
    };
}

HRESULT ParseMarkupExtensionInternal(CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength, CObjectWithType** ppValue, UINT32* pCharactersConsumed)
{
    HRESULT hr = S_OK;
    const WCHAR* pParsePoint = NULL;
    BOOL Continue = TRUE;
    MarkupExtensionParseState::Value ParseState = MarkupExtensionParseState::Start;
    UINT32 MarkupTypeCharacters = 0;
    WCHAR MarkupType[256] = { 0 };
    CClassResolver* pClassResolver = NULL;
    CResolvedClass* pResolvedClass = NULL;
    CPropertyObject* pExtension = NULL;
    UINT32 PropertyNameCharacters = 0;
    WCHAR PropertyName[256] = { 0 };    
    UINT32 PropertyValueCharacters = 0;
    WCHAR PropertyValue[256] = { 0 };    
    CPropertyInformation* pProperties = NULL;
    CProperty* pProperty = NULL;
    CObjectWithType* pPropertyValue = NULL;
    CStringValue* pStringPropertyValue = NULL;

    IFCPTR(pContext);
    IFCPTR(pValue);
    IFCEXPECT(ValueLength > 0);
    IFCPTR(ppValue);
    IFCPTR(pCharactersConsumed);

    pClassResolver = pContext->GetClassResolver();

    IFCPTR(pClassResolver);

    pParsePoint = pValue;

    while(Continue)
    {
        switch(ParseState)
        {
            case MarkupExtensionParseState::Start:
                {
                    IFCEXPECT(pParsePoint < pValue + ValueLength);

                    const WCHAR Token = *pParsePoint;

                    if(Token == L'{')
                    {
                        ++pParsePoint;

                        ParseState = MarkupExtensionParseState::ParseMarkupType;
                    }
                    else
                    {
                        IFC(E_UNEXPECTED);
                    }

                    break;
                }

            case MarkupExtensionParseState::ParseMarkupType:
                {
                    IFCEXPECT(pParsePoint < pValue + ValueLength);

                    const WCHAR Token = *pParsePoint;

                    if(iswalnum(Token))
                    {
                        IFCEXPECT(MarkupTypeCharacters < ARRAYSIZE(MarkupType) - 1);

                        MarkupType[MarkupTypeCharacters] = Token;

                        ++MarkupTypeCharacters;

                        ++pParsePoint;
                    }
                    else if(iswspace(Token))
                    {
                        ParseState = MarkupExtensionParseState::CreateMarkupType;
                    }
                    else
                    {
                        IFC(E_UNEXPECTED);
                    }

                    break;
                }

            case MarkupExtensionParseState::CreateMarkupType:
                {
                    IFCEXPECT(MarkupTypeCharacters > 0);
                    IFCEXPECT(MarkupTypeCharacters < ARRAYSIZE(MarkupType) - 1);

                    MarkupType[MarkupTypeCharacters] = L'\0';

                    IFC(pClassResolver->ResolveType(MarkupType, &pResolvedClass));

                    IFC(pResolvedClass->CreateInstance(&pExtension));

                    IFC(pResolvedClass->GetPropertyInformation(&pProperties));

                    ParseState = MarkupExtensionParseState::BeginFindProperty;

                    break;
                }

            case MarkupExtensionParseState::BeginFindProperty:
                {
                    IFCEXPECT(pParsePoint < pValue + ValueLength);

                    const WCHAR Token = *pParsePoint;

                    if(Token == L'}')
                    {
                        ++pParsePoint;

                        ParseState = MarkupExtensionParseState::Complete;
                    }
                    else if(iswspace(Token))
                    {
                        ParseState = MarkupExtensionParseState::FindProperty;
                    }
                    else
                    {
                        IFC(E_UNEXPECTED);
                    }

                    break;
                }

            case MarkupExtensionParseState::FindProperty:
                {
                    IFCEXPECT(pParsePoint < pValue + ValueLength);

                    const WCHAR Token = *pParsePoint;

                    if(iswalnum(Token))
                    {
                        ParseState = MarkupExtensionParseState::ParseProperty;
                    }
                    else if(iswspace(Token))
                    {
                        ++pParsePoint;
                    }
                    else
                    {
                        IFC(E_UNEXPECTED);
                    }

                    break;
                }

            case MarkupExtensionParseState::ParseProperty:
                {
                    IFCEXPECT(pParsePoint < pValue + ValueLength);

                    const WCHAR Token = *pParsePoint;

                    if(iswalnum(Token))
                    {
                        IFCEXPECT(PropertyNameCharacters < ARRAYSIZE(PropertyName) - 1);

                        PropertyName[PropertyNameCharacters] = Token;

                        ++PropertyNameCharacters;

                        ++pParsePoint;
                    }
                    else if(Token == L'=')
                    {
                        IFCEXPECT(PropertyNameCharacters < ARRAYSIZE(PropertyName) - 1);

                        PropertyName[PropertyNameCharacters] = L'\0';

                        IFC(pProperties->GetProperty(PropertyName, &pProperty));

                        ++pParsePoint;

                        ParseState = MarkupExtensionParseState::BeginParsePropertyValue;
                    }
                    else
                    {
                        IFC(E_UNEXPECTED);
                    }

                    break;
                }

            case MarkupExtensionParseState::BeginParsePropertyValue:
                {
                    IFCEXPECT(pParsePoint < pValue + ValueLength);

                    const WCHAR Token = *pParsePoint;

                    if(Token == L'{')
                    {
                        UINT32 CharactersConsumed = 0;

                        IFC(ParseMarkupExtensionInternal(pContext, pParsePoint, (pValue + ValueLength) - pParsePoint, &pPropertyValue, &CharactersConsumed));

                        IFC(AssignProperty(pExtension, pProperty, pPropertyValue, pContext->GetTypeConverter()));

                        ReleaseObject(pPropertyValue);

                        pParsePoint += CharactersConsumed;

                        ParseState = MarkupExtensionParseState::BeginFindProperty;
                    }
                    else if(iswalnum(Token))
                    {
                        ParseState = MarkupExtensionParseState::ParsePropertyValue;
                    }
                    else
                    {
                        IFC(E_UNEXPECTED);
                    }

                    break;
                }

            case MarkupExtensionParseState::ParsePropertyValue:
                {
                    IFCEXPECT(pParsePoint < pValue + ValueLength);

                    const WCHAR Token = *pParsePoint;

                    if(iswalnum(Token))
                    {
                        IFCEXPECT(PropertyValueCharacters < ARRAYSIZE(PropertyValue) - 1);

                        PropertyValue[PropertyValueCharacters] = Token;

                        ++PropertyValueCharacters;

                        ++pParsePoint;
                    }
                    else if(iswspace(Token) || Token == L'}')
                    {
                        IFCEXPECT(PropertyValueCharacters < ARRAYSIZE(PropertyValue) - 1);

                        PropertyValue[PropertyValueCharacters] = L'\0';

                        ++PropertyValueCharacters;

                        IFC(CStringValue::Create(PropertyValue, &pStringPropertyValue));

                        IFC(AssignProperty(pExtension, pProperty, pStringPropertyValue, pContext->GetTypeConverter()));

                        ReleaseObject(pStringPropertyValue);

                        ParseState = MarkupExtensionParseState::BeginFindProperty;
                    }
                    else
                    {
                        IFC(E_UNEXPECTED);
                    }

                    break;
                }

            case MarkupExtensionParseState::Complete:
                {
                    *ppValue = pExtension;
                    pExtension = NULL;

                    *pCharactersConsumed = pParsePoint - pValue;

                    Continue = FALSE;

                    break;
                }
        }
    }

Cleanup:
    ReleaseObject(pResolvedClass);
    ReleaseObject(pExtension);
    ReleaseObject(pProperty);
    ReleaseObject(pProperties);
    ReleaseObject(pPropertyValue);
    ReleaseObject(pStringPropertyValue);

    return hr;
}

HRESULT ParseMarkupExtension(CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    UINT32 ConsumedCharacters = 0;

    IFCPTR(pContext);
    IFCPTR(pValue);
    IFCEXPECT(ValueLength > 0);
    IFCPTR(ppValue);

    IFC(ParseMarkupExtensionInternal(pContext, pValue, ValueLength, ppValue, &ConsumedCharacters));

    IFCEXPECT(ConsumedCharacters == ValueLength);

Cleanup:
    return hr;
}

HRESULT AttributeStringToValue(CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    IFCPTR(pContext);
    IFCPTR(pValue);
    IFCPTR(ppValue);

    if(SUCCEEDED(ParseMarkupExtension(pContext, pValue, ValueLength, ppValue)))
    {
        goto Cleanup;
    }
    else
    {
        IFC(CStringValue::Create(pValue, ValueLength, &pStringValue));

        *ppValue = pStringValue;
        pStringValue = NULL;
    }

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

HRESULT AssignProperty(CPropertyObject* pElement, CProperty* pProperty, CObjectWithType* pValue, CTypeConverter* pTypeConverter, CObjectWithType* pKey)
{
    HRESULT hr = S_OK;
    CObjectWithType* pConvertedType = NULL;
    CObjectWithType* pCollectionObject = NULL;
    CObjectCollection* pCollection = NULL;
    CObjectWithType* pDictionaryObject = NULL;
    CObjectDictionary* pDictionary = NULL;

    IFCPTR(pElement);
    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pKey)
    {
        IFCEXPECT(pProperty->IsDictionary());
    }

    if(pValue->IsTypeOf(pProperty->GetType()) || pValue->IsTypeOf(TypeIndex::Binding))
    {
        pConvertedType = pValue;
        AddRefObject(pConvertedType);
    }
    else
    {
        IFCPTR(pTypeConverter);

        IFC(pTypeConverter->Convert(pValue, pProperty->GetType(), &pConvertedType));
    }

    if(pProperty->IsDictionary())
    {
        IFCPTR(pKey);

        IFC(pElement->GetValue(pProperty, &pDictionaryObject));

        IFCPTR(pDictionaryObject);

        IFCEXPECT(pDictionaryObject->IsTypeOf(TypeIndex::Dictionary));

        pDictionary = (CObjectDictionary*)pDictionaryObject;

        IFC(pDictionary->AddObject(pKey, pConvertedType));
    }
    else if(pProperty->IsCollection())
    {
        IFC(pElement->GetValue(pProperty, &pCollectionObject));

        IFCPTR(pCollectionObject);

        IFCEXPECT(pCollectionObject->IsTypeOf(TypeIndex::Collection));

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
    ReleaseObject(pDictionaryObject);

    return hr;
}