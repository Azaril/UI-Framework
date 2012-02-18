#include "AddToCollectionCommand.h"

CAddToCollectionCommand::CAddToCollectionCommand(
	) 
	: m_Property(NULL)
{
}

CAddToCollectionCommand::~CAddToCollectionCommand(
	)
{
    ReleaseObject(m_Property);
}

__checkReturn HRESULT 
CAddToCollectionCommand::Initialize(
	__in CProperty* pProperty
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
CAddToCollectionCommand::Execute(
	CParserCommandContext& Context
	)
{
    HRESULT hr = S_OK;
    CObjectWithType* pValueObject = NULL;
    CPropertyObject* pCollectionParent = NULL;
    CObjectWithType* pCollectionObject = NULL;
    CObjectCollection* pCollection = NULL;
    CResolvedClass* pResolvedCollectionType = NULL;
    CPropertyObject* pNewCollection = NULL;

    IFC(Context.GetObject(&pValueObject));

    IFC(Context.PopObject());

    IFC(Context.GetObject(&pCollectionParent));

    IFC(pCollectionParent->GetValue(m_Property, &pCollectionObject));

    if (pCollectionObject == NULL)
    {
        IFC(Context.GetProviders()->GetClassResolver()->ResolveType(m_Property->GetType(), &pResolvedCollectionType));

        IFC(pResolvedCollectionType->CreateInstance(Context.GetProviders(), &pNewCollection));

        IFC(pCollectionParent->SetValue(m_Property, pNewCollection));

        pCollectionObject = pNewCollection;
        pNewCollection = NULL;
    }

    IFC(CastType(pCollectionObject, &pCollection));

    IFC(pCollection->AddObject(pValueObject));

Cleanup:
    ReleaseObject(pCollectionParent);
    ReleaseObject(pCollectionObject);
    ReleaseObject(pValueObject);
    ReleaseObject(pResolvedCollectionType);
    ReleaseObject(pNewCollection);

    return hr;
}