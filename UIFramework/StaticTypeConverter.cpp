#include "StaticTypeConverter.h"

CStaticTypeConverter::CStaticTypeConverter(
    )
    : m_ConverterInformation(NULL)
{
}

CStaticTypeConverter::~CStaticTypeConverter(
    )
{
}

__checkReturn HRESULT 
CStaticTypeConverter::Initialize(
    __in StaticTypeConverterInformation* pConverterInformation
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pConverterInformation);

    m_ConverterInformation = pConverterInformation;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CStaticTypeConverter::Convert(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue,
    __deref_out CObjectWithType** ppConvertedValue
    )
{
    HRESULT hr = S_OK;
    TypeIndex::Value SourceType;
    TypeIndex::Value TargetType;

    IFCPTR(pValue);
    IFCPTR(ppConvertedValue);

    SourceType = pValue->GetType();
    TargetType = pContext->GetTargetType();

    if(SourceType == TargetType)
    {
        *ppConvertedValue = pValue;
        AddRefObject(pValue);
    }
    else
    {
        for(UINT32 i = 0; i < m_ConverterInformation->ConverterCount; i++)
        {
            if(m_ConverterInformation->Converters[i].FromType == SourceType && m_ConverterInformation->Converters[i].ToType == TargetType)
            {
                IFC(m_ConverterInformation->Converters[i].Convert(pContext, pValue, ppConvertedValue));

                goto Cleanup;
            }
        }

        IFC(E_FAIL);
    }

Cleanup:
    return hr;
}