#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Factory.h"
#include "BasicTypes.h"
#include "StaticPropertyInformation.h"
#include "Collection.h"
#include "Providers.h"
#include "StyleCallback.h"

class CResolvedSetter;
class CUIElement;

class CSetter : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY( CSetter );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Setter, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        const WCHAR* GetPropertyName();
        CObjectWithType* GetPropertyValue();

        virtual HRESULT ResolveSetter( CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback, CResolvedSetter** ppResolvedSetter );

        //
        // Properties
        //
        static CStaticProperty PropertyProperty;
        static CStaticProperty ValueProperty;

    protected:
        CSetter();
        virtual ~CSetter();

        HRESULT Initialize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

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

class CResolvedSetter : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CResolvedSetter, CProperty*, CObjectWithType*, IStyleCallback* );

        virtual HRESULT Apply();

    protected:
        CResolvedSetter();
        virtual ~CResolvedSetter();

        HRESULT Initialize( CProperty* pProperty, CObjectWithType* pValue, IStyleCallback* pCallback );

        CProperty* m_Property;
        CObjectWithType* m_Value;
        IStyleCallback* m_Callback;
};