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
    CTypeConverter* pTypeConverter = NULL;
    CObjectWithType* pConvertedType = NULL;
    CBindingBase* pBinding = NULL;

    IFC(Context.GetObject(&pValue));

    IFC(Context.PopObject());

    IFC(Context.GetObject(&pParent));

    if(pValue->IsTypeOf(TypeIndex::BindingBase))
    {
        IFC(CastType(pValue, &pBinding));

        IFC(pParent->SetBinding(m_Property, pBinding));
    }
    else
    {
        if(pValue->IsTypeOf(m_Property->GetType()))
        {
            pConvertedType = pValue;
            AddRefObject(pConvertedType);
        }
        else
        {
            CConversionContext ConversionContext(pParent, m_Property, Context.GetProviders());

            pTypeConverter = Context.GetProviders()->GetTypeConverter();
            IFCPTR(pTypeConverter);

            IFC(pTypeConverter->Convert(&ConversionContext, pValue, &pConvertedType));
        }

        IFC(pParent->SetValue(m_Property, pConvertedType));
    }

Cleanup:
    ReleaseObject(pParent);
    ReleaseObject(pValue);
    ReleaseObject(pConvertedType);

    return hr;
}