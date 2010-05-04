#pragma once

#include "PropertyObject.h"
#include "Factory.h"

namespace StaticPropertyFlags
{
    enum Value
    {
        None        = 0x00,
        Content     = 0x01,
        Collection  = 0x02,
        Attached    = 0x04
    };
}

class CStaticProperty : public CProperty
{
    public:
        CStaticProperty( const WCHAR* Name, const TypeIndex::Value Type, UINT32 Flags );

        virtual INT32 AddRef();
        virtual INT32 Release();

        virtual TypeIndex::Value GetType();
        virtual const WCHAR* GetName();
        virtual BOOL IsCollection();
        virtual BOOL IsAttached();
        BOOL IsContent();

    protected:
        const WCHAR* m_Name;
        TypeIndex::Value m_Type;
        UINT32 m_Flags;
};

class CStaticPropertyInformation : public CPropertyInformation
{
    public:
        DECLARE_FACTORY2( CStaticPropertyInformation, CStaticProperty*, UINT32 );

        virtual HRESULT GetProperty( const WCHAR* pPropertyName, CProperty** ppProperty );
        virtual HRESULT GetContentProperty( CProperty** ppProperty );

    public:
        CStaticPropertyInformation();
        virtual ~CStaticPropertyInformation();

        HRESULT Initialize( CStaticProperty* pProperties, UINT32 PropertyCount );

        CStaticProperty* m_Properties;
        UINT32 m_PropertyCount;
};
