#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Factory.h"
#include "Collection.h"
#include "Setter.h"
#include "Providers.h"
#include "StaticPropertyInformation.h"

class CSetterCollection : public CCollection< CSetter >
{
    public:
        DECLARE_FACTORY( CSetterCollection );

        DECLARE_TYPE_WITH_BASE( TypeIndex::SetterCollection, CCollection< CSetter > );
};

template< >
struct ObjectTypeTraits< CSetterCollection >
{
    static const TypeIndex::Value Type = TypeIndex::SetterCollection;
};

struct IResolvedStyleSetterCallback
{
    virtual HRESULT OnResolvedSetter( CProperty* pProperty, CObjectWithType* pValue ) = 0;
};

class CStyle : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY( CStyle );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Style, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        HRESULT ResolveSetters( CPropertyObject* pObject, CProviders* pProviders, IResolvedStyleSetterCallback* pCallback );

        //
        // Properties
        //
        static CStaticProperty SettersProperty;

    protected:
        CStyle();
        virtual ~CStyle();

        HRESULT Initialize();

        CSetterCollection* m_Setters;
};

template< >
struct ObjectTypeTraits< CStyle >
{
    static const TypeIndex::Value Type = TypeIndex::Style;
};