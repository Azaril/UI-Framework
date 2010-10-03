#pragma once

#include "ClassResolver.h"
#include "TypeConverter.h"
#include "Factory.h"

class CBindingManager;

class UIFRAMEWORK_API CProviders : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CProviders, CClassResolver*, CTypeConverter* );

        CClassResolver* GetClassResolver();
        CTypeConverter* GetTypeConverter();
        CBindingManager* GetBindingManager();

    protected:
        CProviders();
        virtual ~CProviders();

        HRESULT Initialize( CClassResolver* pClassResolver, CTypeConverter* pTypeConverter );

        CClassResolver* m_ClassResolver;
        CTypeConverter* m_TypeConverter;
        CBindingManager* m_BindingManager;
};