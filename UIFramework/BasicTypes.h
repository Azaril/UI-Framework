#pragma once

#include "Types.h"
#include "TypeIndex.h"
#include "Factory.h"
#include "PropertyObject.h"
#include "Enums.h"

template< typename T, TypeIndex::Value TypeID >
class CBasicValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CBasicValue, T );

        DECLARE_TYPE_WITH_BASE( TypeID, CObjectWithType );

        virtual T GetValue()
        {
            return m_Value;
        }

        virtual BOOL Equals( CObjectWithType* pOther )
        {
            if(pOther && pOther->GetType() == GetType())
            {
                CBasicValue< T, TypeID >* pTypedOther = (CBasicValue< T, TypeID >*)pOther;

                return pTypedOther->GetValue() == m_Value;
            }

            return FALSE;
        }

        virtual BOOL IsShareable() { return true; }

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

typedef CBasicValue< RectangleEdge::Value, TypeIndex::RectangleEdge > CRectangleEdgeValue;

template< >
struct ObjectTypeTraits< CRectangleEdgeValue >
{
    static const TypeIndex::Value Type = TypeIndex::RectangleEdge;
};

typedef CBasicValue< bool, TypeIndex::Bool > CBoolValue;

template< >
struct ObjectTypeTraits< CBoolValue >
{
    static const TypeIndex::Value Type = TypeIndex::Bool;
};

typedef CBasicValue< Point2F, TypeIndex::Point2F > CPoint2FValue;

template< >
struct ObjectTypeTraits< CPoint2FValue >
{
    static const TypeIndex::Value Type = TypeIndex::Point2F;
};

typedef CBasicValue< HorizontalAlignment::Value, TypeIndex::HorizontalAlignment > CHorizontalAlignmentValue;

template< >
struct ObjectTypeTraits< CHorizontalAlignmentValue >
{
    static const TypeIndex::Value Type = TypeIndex::HorizontalAlignment;
};

typedef CBasicValue< VerticalAlignment::Value, TypeIndex::VerticalAlignment > CVerticalAlignmentValue;

template< >
struct ObjectTypeTraits< CVerticalAlignmentValue >
{
    static const TypeIndex::Value Type = TypeIndex::VerticalAlignment;
};

typedef CBasicValue< TypeIndex::Value, TypeIndex::Type > CTypeValue;

template< >
struct ObjectTypeTraits< CTypeValue >
{
    static const TypeIndex::Value Type = TypeIndex::Type;
};

typedef CBasicValue< Orientation::Value, TypeIndex::Orientation > COrientationValue;

template< >
struct ObjectTypeTraits< COrientationValue >
{
    static const TypeIndex::Value Type = TypeIndex::Orientation;
};

typedef CBasicValue< Stretch::Value, TypeIndex::Stretch > CStretchValue;

template< >
struct ObjectTypeTraits< CStretchValue >
{
    static const TypeIndex::Value Type = TypeIndex::Stretch;
};

typedef CBasicValue< StretchDirection::Value, TypeIndex::StretchDirection > CStretchDirectionValue;

template< >
struct ObjectTypeTraits< CStretchDirectionValue >
{
    static const TypeIndex::Value Type = TypeIndex::StretchDirection;
};

class CStringValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CStringValue, const WCHAR* );
        DECLARE_FACTORY2( CStringValue, const WCHAR*, UINT32 );

        DECLARE_TYPE_WITH_BASE( TypeIndex::String, CObjectWithType );

        const WCHAR* GetValue();
        UINT32 GetLength();

        virtual BOOL Equals( CObjectWithType* pOther );

        HRESULT Clone( CStringValue** ppClone );

    protected:
        CStringValue();
        virtual ~CStringValue();

        HRESULT Initialize( const WCHAR* pValue );
        HRESULT Initialize( const WCHAR* pValue, UINT32 Length );

        WCHAR* m_Value;
        UINT32 m_Length;
};

template< >
struct ObjectTypeTraits< CStringValue >
{
    static const TypeIndex::Value Type = TypeIndex::String;
};