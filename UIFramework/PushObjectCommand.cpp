#include "PushObjectCommand.h"

CPushObjectCommand::CPushObjectCommand(
	) 
	: m_Value(NULL)
{
}

CPushObjectCommand::~CPushObjectCommand(
	)
{
    ReleaseObject(m_Value);
}

__checkReturn HRESULT 
CPushObjectCommand::Initialize(
	__in CObjectWithType* pValue
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pValue);

    m_Value = pValue;
    AddRefObject(m_Value);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CPushObjectCommand::Execute(
	CParserCommandContext& Context
	)
{
    HRESULT hr = S_OK;

    IFC(Context.PushObject(m_Value));

Cleanup:
    return hr;
}
