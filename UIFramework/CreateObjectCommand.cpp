#include "CreateObjectCommand.h"
#include "PropertyObject.h"
#include "Collection.h"

CCreateObjectCommand::CCreateObjectCommand(
	) 
	: m_ResolvedClass(NULL)
{
}

CCreateObjectCommand::~CCreateObjectCommand(
	)
{
    ReleaseObject(m_ResolvedClass);
}

__checkReturn HRESULT 
CCreateObjectCommand::Initialize(
	__in CResolvedClass* pResolvedClass
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pResolvedClass);

    m_ResolvedClass = pResolvedClass;
    AddRefObject(m_ResolvedClass);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CCreateObjectCommand::Execute(
	CParserCommandContext& Context
	)
{
    HRESULT hr = S_OK;
    CPropertyObject* pObject = NULL;

    IFC(m_ResolvedClass->CreateInstance(Context.GetProviders(), &pObject));

    IFC(Context.PushObject(pObject));

Cleanup:
    ReleaseObject(pObject);

    return hr;
}