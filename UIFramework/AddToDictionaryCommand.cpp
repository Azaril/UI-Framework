#include "AddToDictionaryCommand.h"

CAddToDictionaryCommand::CAddToDictionaryCommand(
	) 
	: m_Property(NULL)
{
}

CAddToDictionaryCommand::~CAddToDictionaryCommand(
	)
{
    ReleaseObject(m_Property);
}

__checkReturn HRESULT 
CAddToDictionaryCommand::Initialize(
	CProperty* pProperty
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);

    m_Property = pProperty;
    AddRefObject(m_Property);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CAddToDictionaryCommand::Execute(
	CParserCommandContext& Context
	)
{
    HRESULT hr = S_OK;
    CObjectWithType* pKey = NULL;
    CObjectWithType* pValue = NULL;
    CPropertyObject* pDictionaryParent = NULL;
    CObjectWithType* pDictionaryObject = NULL;
    CObjectDictionary* pDictionary = NULL;
    CResolvedClass* pResolvedCollectionType = NULL;
    CPropertyObject* pNewDictionary = NULL;

    IFC(Context.GetObject(&pKey));

    IFC(Context.PopObject());

    IFC(Context.GetObject(&pValue));

    IFC(Context.PopObject());

    IFC(Context.GetObject(&pDictionaryParent));

    IFC(pDictionaryParent->GetValue(m_Property, &pDictionaryObject));

    if (pDictionaryObject == NULL)
    {
        IFC(Context.GetProviders()->GetClassResolver()->ResolveType(m_Property->GetType(), &pResolvedCollectionType));

        IFC(pResolvedCollectionType->CreateInstance(Context.GetProviders(), &pNewDictionary));

        IFC(pDictionaryParent->SetValue(m_Property, pNewDictionary));

        pDictionaryObject = pNewDictionary;
        pNewDictionary = NULL;
    }

    IFC(CastType(pDictionaryObject, &pDictionary));

    IFC(pDictionary->AddObject(pKey, pValue));

Cleanup:
    ReleaseObject(pKey);
    ReleaseObject(pValue);
    ReleaseObject(pDictionaryParent);
    ReleaseObject(pDictionaryObject);
    ReleaseObject(pNewDictionary);
    ReleaseObject(pResolvedCollectionType);

    return hr;
}