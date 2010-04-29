#pragma once

#include "TypeConverter.h"
#include "Factory.h"

typedef HRESULT (*ConvertTypeFunc)( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );

struct StaticTypeConverter
{
    TypeIndex::Value FromType;
    TypeIndex::Value ToType;
    ConvertTypeFunc Convert;
};

struct StaticTypeConverterInformation
{
    StaticTypeConverter* Converters;
    UINT32 ConverterCount;
};

class CStaticTypeConverter : public CTypeConverter
{
    public:
        DECLARE_FACTORY1( CStaticTypeConverter, StaticTypeConverterInformation* );

        virtual HRESULT Convert( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );

    protected:
        CStaticTypeConverter();
        virtual ~CStaticTypeConverter();

        HRESULT Initialize( StaticTypeConverterInformation* pConverterInformation );

        StaticTypeConverterInformation* m_ConverterInformation;
};
