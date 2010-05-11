#include "SetPropertyCommand.h"

CSetPropertyCommand::CSetPropertyCommand() : m_Property(NULL)
{
}

CSetPropertyCommand::~CSetPropertyCommand()
{
    ReleaseObject(m_Property);
}

HRESULT CSetPropertyCommand::Initialize(CProperty* pProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);

    m_Property = pProperty;
    AddRefObject(m_Property);

Cleanup:
    return hr;
}

HRESULT CSetPropertyCommand::Execute(CParserCommandContext& Context)
{
    HRESULT hr = S_OK;
    CPropertyObject* pParent = NULL;
    CObjectWithType* pValue = NULL;
    CTypeConverter* pTypeConverter = NULL;
    CObjectWithType* pConvertedType = NULL;

    IFC(Context.GetObject(&pValue));

    IFC(Context.PopObject());

    IFC(Context.GetObject(&pParent));

    if(pValue->IsTypeOf(m_Property->GetType()) || pValue->IsTypeOf(TypeIndex::Binding))
    {
        pConvertedType = pValue;
        AddRefObject(pConvertedType);
    }
    else
    {
        CConversionContext Context(pParent, m_Property, Context.GetProviders());

        pTypeConverter = Context.GetProviders()->GetTypeConverter();
        IFCPTR(pTypeConverter);

        IFC(pTypeConverter->Convert(&Context, pValue, &pConvertedType));
    }

    IFC(pParent->SetValue(m_Property, pConvertedType));

Cleanup:
    ReleaseObject(pParent);
    ReleaseObject(pValue);
    ReleaseObject(pConvertedType);

    return hr;
}