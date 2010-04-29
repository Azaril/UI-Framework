#include "StaticTypeConverter.h"

CStaticTypeConverter::CStaticTypeConverter() : m_ConverterInformation(NULL)
{
}

CStaticTypeConverter::~CStaticTypeConverter()
{
}

HRESULT CStaticTypeConverter::Initialize(StaticTypeConverterInformation* pConverterInformation)
{
    HRESULT hr = S_OK;

    IFCPTR(pConverterInformation);

    m_ConverterInformation = pConverterInformation;

Cleanup:
    return hr;
}

HRESULT CStaticTypeConverter::Convert(CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue)
{
    HRESULT hr = S_OK;
    TypeIndex::Value SourceType;

    IFCPTR(pValue);
    IFCPTR(ppConvertedValue);

    SourceType = pValue->GetType();

    for(UINT32 i = 0; i < m_ConverterInformation->ConverterCount; i++)
    {
        if(m_ConverterInformation->Converters[i].FromType == SourceType && m_ConverterInformation->Converters[i].ToType == TargetType)
        {
            IFC(m_ConverterInformation->Converters[i].Convert(pValue, TargetType, ppConvertedValue));

            goto Cleanup;
        }
    }

    IFC(E_FAIL);

Cleanup:
    return hr;
}