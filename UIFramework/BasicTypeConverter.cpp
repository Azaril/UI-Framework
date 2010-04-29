#include "BasicTypeConverter.h"
#include "StaticTypeConverter.h"
#include "TypeIndex.h"
#include "BasicTypes.h"

StaticTypeConverter BasicConverters[] =
{
    { TypeIndex::String, TypeIndex::Float, ConvertStringToFloat }
};

StaticTypeConverterInformation BasicConverterInfo =
{
    BasicConverters,
    ARRAYSIZE(BasicConverters)
};

HRESULT CreateBasicTypeConverter(CTypeConverter** ppTypeConverter)
{
    HRESULT hr = S_OK;
    CStaticTypeConverter* pStaticTypeConverter = NULL;

    IFCPTR(ppTypeConverter);

    IFC(CStaticTypeConverter::Create(&BasicConverterInfo, &pStaticTypeConverter));

    *ppTypeConverter = pStaticTypeConverter;
    pStaticTypeConverter = NULL;

Cleanup:
    ReleaseObject(pStaticTypeConverter);

    return hr;
}

HRESULT ConvertStringToFloat(CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue)
{
    HRESULT hr = S_OK;
    FLOAT Value = 0;
    CStringValue* pStringValue = NULL;
    CFloatValue* pFloatValue = NULL;

    IFCPTR(pValue);
    IFCPTR(ppConvertedValue);

    IFCEXPECT(pValue->GetType() == TypeIndex::String);

    pStringValue = (CStringValue*)pValue;

    Value = (FLOAT)_wtof(pStringValue->GetValue());

    IFC(CFloatValue::Create(Value, &pFloatValue));

    *ppConvertedValue = pFloatValue;
    pFloatValue = NULL;

Cleanup:
    ReleaseObject(pFloatValue);

    return hr;
}