#pragma once

#include "TypeConverter.h"
#include "Factory.h"

typedef __checkReturn HRESULT (*ConvertTypeFunc)(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

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

        __override virtual __checkReturn HRESULT Convert( 
            __in CConversionContext* pContext, 
            __in CObjectWithType* pValue,
            __deref_out CObjectWithType** ppConvertedValue 
            );

    protected:
        CStaticTypeConverter(
            );

        virtual ~CStaticTypeConverter(
            );

        __checkReturn HRESULT Initialize( 
            __in StaticTypeConverterInformation* pConverterInformation 
            );

        StaticTypeConverterInformation* m_ConverterInformation;
};
