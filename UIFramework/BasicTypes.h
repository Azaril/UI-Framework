#pragma once

#include "Types.h"
#include "TypeIndex.h"
#include "Factory.h"
#include "PropertyObject.h"

class CFloatValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CFloatValue, FLOAT );

        virtual TypeIndex::Value GetType() { return TypeIndex::Float; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Float; }

        virtual FLOAT GetValue();

    protected:
        HRESULT Initialize( FLOAT Value );

        FLOAT m_Value;
};

class CStringValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CStringValue, const WCHAR* );
        DECLARE_FACTORY2( CStringValue, const WCHAR*, UINT32 );

        virtual TypeIndex::Value GetType() { return TypeIndex::String; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::String; }

        const WCHAR* GetValue();

    protected:
        CStringValue();
        virtual ~CStringValue();

        HRESULT Initialize( const WCHAR* pValue );
        HRESULT Initialize( const WCHAR* pValue, UINT32 Length );

        WCHAR* m_Value;
};