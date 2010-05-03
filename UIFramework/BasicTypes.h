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

        DECLARE_TYPE_WITH_BASE( Type, CObjectWithType );

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

template< >
struct ObjectTypeTraits< CFloatValue >
{
    static const TypeIndex::Value Type = TypeIndex::Float;
};

typedef CBasicValue< Visibility::Value, TypeIndex::Visibility > CVisibilityValue;

template< >
struct ObjectTypeTraits< CVisibilityValue >
{
    static const TypeIndex::Value Type = TypeIndex::Visibility;
};

typedef CBasicValue< ColorF, TypeIndex::ColorF > CColorFValue;

template< >
struct ObjectTypeTraits< CColorFValue >
{
    static const TypeIndex::Value Type = TypeIndex::ColorF;
};

typedef CBasicValue< RectF, TypeIndex::RectF > CRectFValue;

template< >
struct ObjectTypeTraits< CRectFValue >
{
    static const TypeIndex::Value Type = TypeIndex::RectF;
};

class CStringValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CStringValue, const WCHAR* );
        DECLARE_FACTORY2( CStringValue, const WCHAR*, UINT32 );

        DECLARE_TYPE_WITH_BASE( TypeIndex::String, CObjectWithType );

        const WCHAR* GetValue();

    protected:
        CStringValue();
        virtual ~CStringValue();

        HRESULT Initialize( const WCHAR* pValue );
        HRESULT Initialize( const WCHAR* pValue, UINT32 Length );

        WCHAR* m_Value;
};

template< >
struct ObjectTypeTraits< CStringValue >
{
    static const TypeIndex::Value Type = TypeIndex::String;
};