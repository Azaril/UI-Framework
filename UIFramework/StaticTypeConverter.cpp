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

HRESULT CStaticTypeConverter::Convert(CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue)
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