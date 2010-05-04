#pragma once

#include "ClassResolver.h"
#include "TypeConverter.h"
#include "Factory.h"

class CParseContext : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CParseContext, CClassResolver*, CTypeConverter* );

        CClassResolver* GetClassResolver();
        CTypeConverter* GetTypeConverter();

    protected:
        CParseContext();
        virtual ~CParseContext();

        HRESULT Initialize( CClassResolver* pClassResolver, CTypeConverter* );

        CClassResolver* m_ClassResolver;
        CTypeConverter* m_TypeConverter;
};