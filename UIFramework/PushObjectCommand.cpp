#include "PushObjectCommand.h"

CPushObjectCommand::CPushObjectCommand() : m_Value(NULL)
{
}

CPushObjectCommand::~CPushObjectCommand()
{
    ReleaseObject(m_Value);
}

HRESULT CPushObjectCommand::Initialize(CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pValue);

    m_Value = pValue;
    AddRefObject(m_Value);

Cleanup:
    return hr;
}

HRESULT CPushObjectCommand::Execute(CParserCommandContext& Context)
{
    HRESULT hr = S_OK;

    IFC(Context.PushObject(m_Value));

Cleanup:
    return hr;
}
