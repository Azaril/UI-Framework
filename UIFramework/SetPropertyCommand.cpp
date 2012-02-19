#include "SetPropertyCommand.h"
#include "BindingBase.h"

CSetPropertyCommand::CSetPropertyCommand(
	) 
	: m_Property(NULL)
{
}

CSetPropertyCommand::~CSetPropertyCommand(
	)
{
    ReleaseObject(m_Property);
}

__checkReturn HRESULT 
CSetPropertyCommand::Initialize(
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
CSetPropertyCommand::Execute(
	CParserCommandContext& Context
	)
{
    HRESULT hr = S_OK;
    CPropertyObject* pParent = NULL;
    CObjectWithType* pValue = NULL;
    CBindingBase* pBinding = NULL;

    IFC(Context.GetObject(&pValue));

    IFC(Context.PopObject());

    IFC(Context.GetObject(&pParent));

    if(pValue->IsTypeOf(TypeIndex::BindingBase))
    {
        IFC(CastType(pValue, &pBinding));

        //TODO: Move this to SetValue?
        IFC(pParent->SetBinding(m_Property, pBinding));
    }
    else
    {
        IFC(pParent->SetValue(m_Property, pValue));
    }

Cleanup:
    ReleaseObject(pParent);
    ReleaseObject(pValue);

    return hr;
}