#pragma once

#include "TypeConverter.h"
#include "Factory.h"

typedef HRESULT (*ConvertTypeFunc)( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );

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

        virtual HRESULT Convert( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );

    protected:
        CStaticTypeConverter();
        virtual ~CStaticTypeConverter();

        HRESULT Initialize( StaticTypeConverterInformation* pConverterInformation );

        StaticTypeConverterInformation* m_ConverterInformation;
};
