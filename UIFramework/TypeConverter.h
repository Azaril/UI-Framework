#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"
#include "ClassResolver.h"

class CConversionContext
{
    public:
        CConversionContext( CPropertyObject* pTargetObject, CProperty* pTargetProperty, CClassResolver* pClassResolver );

        CPropertyObject* GetTargetObject();
        CProperty* GetTargetProperty();
        CClassResolver* GetClassResolver();
        TypeIndex::Value GetTargetType();

    protected:
        CPropertyObject* m_TargetObject;
        CProperty* m_TargetProperty;
        CClassResolver* m_ClassResolver;
};

class CTypeConverter : public CRefCountedObject
{
    public:
        virtual HRESULT Convert( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue ) = 0;
};
