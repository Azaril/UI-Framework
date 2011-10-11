#pragma once

#include "ClassResolver.h"
#include "TypeConverter.h"
#include "Factory.h"

class CBindingManager;
struct IResourceProvider;

class UIFRAMEWORK_API CProviders : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CProviders, CClassResolver*, CTypeConverter*, IResourceProvider* );

        __out CClassResolver* GetClassResolver(
            );

        __out CTypeConverter* GetTypeConverter(
            );

        __out CBindingManager* GetBindingManager(
            );

        __out IResourceProvider* GetResourceProvider(
            );

    protected:
        CProviders(
            );

        virtual ~CProviders(
            );

        __checkReturn HRESULT Initialize( 
            __in CClassResolver* pClassResolver,
            __in CTypeConverter* pTypeConverter,
            __in IResourceProvider* pResourceProvider
            );

        CClassResolver* m_pClassResolver;
        CTypeConverter* m_pTypeConverter;
        CBindingManager* m_pBindingManager;
        IResourceProvider* m_pResourceProvider;
};