#include "ParserUtilities.h"
#include "RichPropertyNodeCallback.h"
#include "ContentPropertyNodeCallback.h"
#include "BasicTypes.h"
#include "CreateObjectCommand.h"
#include "PushObjectCommand.h"
#include "AddToCollectionCommand.h"
#include "AddToDictionaryCommand.h"
#include "SetPropertyCommand.h"

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
    CElementNodeCallback* pElementCallback = NULL;

    IFCPTR(pContext);
    IFCPTR(pStart);
    IFCPTR(ppCallback);

    IFC(CElementNodeCallback::Create(pContext, pStart, &pElementCallback));

    *ppCallback = pElementCallback;
    pElementCallback = NULL;

Cleanup:
    ReleaseObject(pElementCallback);

    return hr;
}

HRESULT ElementStartToParserCallback(CParseContext* pContext, CPropertyInformation* pPropertyInformation, CXMLElementStart* pStart, CPropertyNodeCallback** ppCallback)
{
    HRESULT hr = S_OK;
    CRichPropertyNodeCallback* pRichPropertyCallback = NULL;
    CContentPropertyNodeCallback* pContentPropertyCallback = NULL;
    const WCHAR* pElementName = NULL;
    UINT32 ElementNameLength = 0;

    IFCPTR(pStart);
    IFCPTR(pPropertyInformation);
    IFCPTR(ppCallback);

    IFC(pStart->GetName(&pElementName, &ElementNameLength));

    if(IsAttribute(pElementName))
    {
        IFC(CRichPropertyNodeCallback::Create(pContext, pPropertyInformation, pStart, &pRichPropertyCallback));

        *ppCallback = pRichPropertyCallback;
        pRichPropertyCallback = NULL;
    }
    else
    {
        IFC(CContentPropertyNodeCallback::Create(pContext, pPropertyInformation, pStart, &pContentPropertyCallback));

        *ppCallback = pContentPropertyCallback;
        pContentPropertyCallback = NULL;
    }

Cleanup:
    ReleaseObject(pRichPropertyCallback);
    ReleaseObject(pContentPropertyCallback);

    return hr;
}

HRESULT TextToParserCallback(CParseContext* pContext, CPropertyInformation* pPropertyInformation, CXMLText* pText, CPropertyNodeCallback** ppCallback)
{
    HRESULT hr = S_OK;
    CContentPropertyNodeCallback* pContentPropertyCallback = NULL;

    IFCPTR(pContext);
    IFCPTR(pPropertyInformation);
    IFCPTR(pText);
    IFCPTR(ppCallback);

    IFC(CContentPropertyNodeCallback::Create(pContext, pPropertyInformation, pText, &pContentPropertyCallback));

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

HRESULT ParseMarkupExtensionInternal(CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength, UINT32* pCharactersConsumed)
{
    HRESULT hr = S_OK;
    const WCHAR* pParsePoint = NULL;
    BOOL Continue = TRUE;
    MarkupExtensionParseState::Value ParseState = MarkupExtensionParseState::Start;
    UINT32 MarkupTypeCharacters = 0;
    WCHAR MarkupType[256] = { 0 };
    CClassResolver* pClassResolver = NULL;
    CResolvedClass* pResolvedClass = NULL;
    UINT32 PropertyNameCharacters = 0;
    WCHAR PropertyName[256] = { 0 };    
    UINT32 PropertyValueCharacters = 0;
    WCHAR PropertyValue[256] = { 0 };    
    CPropertyInformation* pProperties = NULL;
    CProperty* pProperty = NULL;
    CStringValue* pStringPropertyValue = NULL;

    IFCPTR(pContext);
    IFCPTR(pValue);
    IFCEXPECT(ValueLength > 0);
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

                    IFC(AddCreateObjectCommand(pContext, pResolvedClass));

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

                        IFC(ParseMarkupExtensionInternal(pContext, pParsePoint, (pValue + ValueLength) - pParsePoint, &CharactersConsumed));

                        IFC(AddSetPropertyCommand(pContext, pProperty));

                        ReleaseObject(pProperty);

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

                        IFC(AddPushValueCommand(pContext, pStringPropertyValue));

                        IFC(AddSetPropertyCommand(pContext, pProperty));

                        ReleaseObject(pProperty);

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
                    *pCharactersConsumed = pParsePoint - pValue;

                    Continue = FALSE;

                    break;
                }
        }
    }

Cleanup:
    ReleaseObject(pResolvedClass);
    ReleaseObject(pProperty);
    ReleaseObject(pProperties);
    ReleaseObject(pStringPropertyValue);

    return hr;
}

HRESULT ParseMarkupExtension(CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength)
{
    HRESULT hr = S_OK;
    UINT32 ConsumedCharacters = 0;

    IFCPTR(pContext);
    IFCPTR(pValue);
    IFCEXPECT(ValueLength > 0);

    IFC(ParseMarkupExtensionInternal(pContext, pValue, ValueLength, &ConsumedCharacters));

    IFCEXPECT(ConsumedCharacters == ValueLength);

Cleanup:
    return hr;
}

HRESULT AddPushValueCommand(CParseContext* pContext, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CPushObjectCommand* pPushObjectCommand = NULL;

    IFCPTR(pContext);
    IFCPTR(pValue);

    IFC(CPushObjectCommand::Create(pValue, &pPushObjectCommand));

    IFC(pContext->AddCommand(pPushObjectCommand));

Cleanup:
    ReleaseObject(pPushObjectCommand);

    return hr;
}

HRESULT AddSetPropertyCommand(CParseContext* pContext, CProperty* pProperty, CStringValue* pKeyString)
{
    HRESULT hr = S_OK;
    CAddToDictionaryCommand* pAddToDictionaryCommand = NULL;
    CAddToCollectionCommand* pAddToCollectionCommand = NULL;
    CSetPropertyCommand* pSetPropertyCommand = NULL;

    IFCPTR(pContext);
    IFCPTR(pProperty);

    if(pProperty->IsDictionary())
    {
        IFCPTR(pKeyString);

        IFC(EvaluateAndAddAttribute(pContext, pKeyString->GetValue(), pKeyString->GetLength()));

        IFC(CAddToDictionaryCommand::Create(pProperty, &pAddToDictionaryCommand));

        IFC(pContext->AddCommand(pAddToDictionaryCommand));
    }
    else if(pProperty->IsCollection())
    {
        IFCEXPECT(pKeyString == NULL);

        IFC(CAddToCollectionCommand::Create(pProperty, &pAddToCollectionCommand));

        IFC(pContext->AddCommand(pAddToCollectionCommand));
    }
    else
    {
        IFCEXPECT(pKeyString == NULL);

        IFC(CSetPropertyCommand::Create(pProperty, &pSetPropertyCommand));

        IFC(pContext->AddCommand(pSetPropertyCommand));
    }

Cleanup:
    ReleaseObject(pAddToDictionaryCommand);
    ReleaseObject(pAddToCollectionCommand);
    ReleaseObject(pSetPropertyCommand);

    return hr;
}

BOOL IsMarkupExtension(const WCHAR* pValue, UINT32 ValueLength)
{
    if(ValueLength > 0 && pValue)
    {
        //TODO: This should be more robust and support escaping.
        return pValue[0] == L'{';
    }

    return FALSE;
}

HRESULT EvaluateAndAddAttribute(CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    if(IsMarkupExtension(pValue, ValueLength))
    {
        IFC(ParseMarkupExtension(pContext, pValue, ValueLength));
    }
    else
    {
        IFC(CStringValue::Create(pValue, ValueLength, &pStringValue));

        IFC(AddPushValueCommand(pContext, pStringValue));
    }

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

HRESULT AddCreateObjectCommand(CParseContext* pContext, CResolvedClass* pClass)
{
    HRESULT hr = S_OK;
    CCreateObjectCommand* pCreateObjectCommand = NULL;

    IFCPTR(pContext);
    IFCPTR(pClass);

    IFC(CCreateObjectCommand::Create(pClass, &pCreateObjectCommand));

    IFC(pContext->AddCommand(pCreateObjectCommand));

Cleanup:
    ReleaseObject(pCreateObjectCommand);

    return hr;
}