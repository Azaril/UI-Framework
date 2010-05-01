#pragma once

#include "Types.h"
#include "TypeIndex.h"
#include "Factory.h"
#include "PropertyObject.h"
#include "Enums.h"

template< typename T, TypeIndex::Value Type >
class CBasicValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CBasicValue, T );

        virtual TypeIndex::Value GetType() { return Type; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == Type || CObjectWithType::IsTypeOf(Type); }

        virtual T GetValue()
        {
            return m_Value;
        }

    protected:
        HRESULT Initialize( T Value )
        {
            m_Value = Value;

            return S_OK;
        }

        T m_Value;
};

typedef CBasicValue< FLOAT, TypeIndex::Float > CFloatValue;
typedef CBasicValue< Visibility::Value, TypeIndex::Visibility > CVisibilityValue;
typedef CBasicValue< ColorF, TypeIndex::ColorF > CColorFValue;
typedef CBasicValue< RectF, TypeIndex::RectF > CRectFValue;

class CStringValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CStringValue, const WCHAR* );
        DECLARE_FACTORY2( CStringValue, const WCHAR*, UINT32 );

        virtual TypeIndex::Value GetType() { return TypeIndex::String; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::String || CObjectWithType::IsTypeOf(Type); }

        const WCHAR* GetValue();

    protected:
        CStringValue();
        virtual ~CStringValue();

        HRESULT Initialize( const WCHAR* pValue );
        HRESULT Initialize( const WCHAR* pValue, UINT32 Length );

        WCHAR* m_Value;
};