#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"
#include "ClassResolver.h"

class CProviders;

class CConversionContext
{
    public:
        CConversionContext( CPropertyObject* pTargetObject, CProperty* pTargetProperty, CProviders* pProviders );

        CPropertyObject* GetTargetObject();
        CProperty* GetTargetProperty();
        CProviders* GetProviders();
        TypeIndex::Value GetTargetType();

    protected:
        CPropertyObject* m_TargetObject;
        CProperty* m_TargetProperty;
        CProviders* m_Providers;
};

class CTypeConverter : public CRefCountedObject
{
    public:
        virtual HRESULT Convert( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue ) = 0;
};
