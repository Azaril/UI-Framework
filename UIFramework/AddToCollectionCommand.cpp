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

    IFC(Context.GetObject(&pValueObject));

    IFC(Context.PopObject());

    IFC(Context.GetObject(&pCollectionParent));

    IFC(pCollectionParent->GetValue(m_Property, &pCollectionObject));

    IFC(CastType(pCollectionObject, &pCollection));

    IFC(pCollection->AddObject(pValueObject));

Cleanup:
    ReleaseObject(pCollectionParent);
    ReleaseObject(pCollectionObject);
    ReleaseObject(pValueObject);

    return hr;
}