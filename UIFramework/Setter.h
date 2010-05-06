#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Factory.h"
#include "BasicTypes.h"
#include "StaticPropertyInformation.h"

class CSetter : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY( CSetter );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Setter, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        const WCHAR* GetPropertyName();
        CObjectWithType* GetPropertyValue();

        //
        // Properties
        //
        static CStaticProperty PropertyProperty;
        static CStaticProperty ValueProperty;

    protected:
        CSetter();
        virtual ~CSetter();

        HRESULT Initialize();

        HRESULT SetPropertyInternal( const WCHAR* pProperty );
        HRESULT SetPropertyValueInternal( CObjectWithType* pValue );

        CStringValue* m_Property;
        CObjectWithType* m_Value;
};

template< >
struct ObjectTypeTraits< CSetter >
{
    static const TypeIndex::Value Type = TypeIndex::Setter;
};