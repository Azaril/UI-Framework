#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"
#include "ClassResolver.h"

class CProviders;

class CConversionContext
{
    public:
        CConversionContext(
            __in CPropertyObject* pTargetObject, 
            __in CProperty* pTargetProperty, 
            __in CProviders* pProviders 
            );

        __out CPropertyObject* GetTargetObject(
            );

        __out CProperty* GetTargetProperty(
            );

        __out CProviders* GetProviders(
            );

        TypeIndex::Value GetTargetType(
            );

    protected:
        CPropertyObject* m_TargetObject;
        CProperty* m_TargetProperty;
        CProviders* m_Providers;
};

class CTypeConverter : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT Convert( 
            __in CConversionContext* pContext, 
            __in CObjectWithType* pValue,
            __deref_out CObjectWithType** ppConvertedValue
            ) = 0;
};
