#pragma once

#include "PropertyObject.h"
#include "Factory.h"

namespace StaticPropertyFlags
{
    enum Value
    {
        None        = 0x00,
        Content     = 0x01,
        Collection  = 0x02
    };
}

struct StaticClassProperty
{
    WCHAR* PropertyName;
    TypeIndex::Value PropertyType;
    UINT32 Flags;
};

struct StaticClassProperties
{
    StaticClassProperty* Properties;
    UINT32 PropertyCount;
};

class CStaticProperty : public CProperty
{
    public:
        DECLARE_FACTORY1( CStaticProperty, StaticClassProperty* );

        virtual TypeIndex::Value GetType();
        virtual const WCHAR* GetName();
        virtual BOOL IsCollection();

    protected:
        CStaticProperty();
        virtual ~CStaticProperty();

        HRESULT Initialize( StaticClassProperty* pProperty );

        StaticClassProperty* m_Property;
};

class CStaticPropertyInformation : public CPropertyInformation
{
    public:
        DECLARE_FACTORY1( CStaticPropertyInformation, StaticClassProperties* );

        virtual HRESULT GetProperty( const WCHAR* pPropertyName, CProperty** ppProperty );
        virtual HRESULT GetContentProperty( CProperty** ppProperty );

    public:
        CStaticPropertyInformation();
        virtual ~CStaticPropertyInformation();

        HRESULT Initialize( StaticClassProperties* pProperties );

        StaticClassProperties* m_ClassInformation;
};
