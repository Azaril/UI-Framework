#pragma once

#include "Types.h"
#include "TypeIndex.h"
#include "Factory.h"
#include "PropertyObject.h"
#include "Enums.h"
#include "TimeValue.h"

template< typename T >
class CBasicValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CBasicValue, T );

        DECLARE_TYPE_WITH_BASE( ObjectTypeTraits< CBasicValue< T > >::Type, CObjectWithType );

        typedef T ValueType; 

        const T& GetValue(
            )
        {
            return m_Value;
        }

        __override virtual bool Equals(
            __in_opt CObjectWithType* pOther 
            )
        {
            if(pOther != NULL && pOther->GetType() == GetType())
            {
                CBasicValue< T >* pTypedOther = (CBasicValue< T >*)pOther;

                return pTypedOther->GetValue() == m_Value;
            }

            return FALSE;
        }

        __override virtual bool IsShareable(
            ) 
        { 
            return true; 
        }

    protected:
        __checkReturn HRESULT Initialize( 
            const T& Value 
            )
        {
            m_Value = Value;

            return S_OK;
        }

        T m_Value;
};

template< typename T >
class CStaticBasicValue : public CBasicValue< T >
{
    public:
        CStaticBasicValue(
            const T& Value
            )
        {
            this->Initialize(Value);
        }

        STATIC_REFCOUNTING();

        DISABLE_OBJECT_TRACKING();
};

typedef CBasicValue< FLOAT > CFloatValue;

template< >
struct ObjectTypeTraits< CFloatValue >
{
    static const TypeIndex::Value Type = TypeIndex::Float;
};

typedef CBasicValue< Visibility::Value > CVisibilityValue;

template< >
struct ObjectTypeTraits< CVisibilityValue >
{
    static const TypeIndex::Value Type = TypeIndex::Visibility;
};

typedef CBasicValue< ColorF > CColorFValue;

template< >
struct ObjectTypeTraits< CColorFValue >
{
    static const TypeIndex::Value Type = TypeIndex::ColorF;
};

typedef CBasicValue< RectF > CRectFValue;

template< >
struct ObjectTypeTraits< CRectFValue >
{
    static const TypeIndex::Value Type = TypeIndex::RectF;
};

typedef CBasicValue< RectangleEdge::Value > CRectangleEdgeValue;

template< >
struct ObjectTypeTraits< CRectangleEdgeValue >
{
    static const TypeIndex::Value Type = TypeIndex::RectangleEdge;
};

typedef CBasicValue< bool > CBoolValue;

template< >
struct ObjectTypeTraits< CBoolValue >
{
    static const TypeIndex::Value Type = TypeIndex::Bool;
};

typedef CBasicValue< Point2F > CPoint2FValue;

template< >
struct ObjectTypeTraits< CPoint2FValue >
{
    static const TypeIndex::Value Type = TypeIndex::Point2F;
};

typedef CBasicValue< HorizontalAlignment::Value > CHorizontalAlignmentValue;

template< >
struct ObjectTypeTraits< CHorizontalAlignmentValue >
{
    static const TypeIndex::Value Type = TypeIndex::HorizontalAlignment;
};

typedef CBasicValue< VerticalAlignment::Value > CVerticalAlignmentValue;

template< >
struct ObjectTypeTraits< CVerticalAlignmentValue >
{
    static const TypeIndex::Value Type = TypeIndex::VerticalAlignment;
};

typedef CBasicValue< TypeIndex::Value > CTypeValue;

template< >
struct ObjectTypeTraits< CTypeValue >
{
    static const TypeIndex::Value Type = TypeIndex::Type;
};

typedef CBasicValue< Orientation::Value > COrientationValue;

template< >
struct ObjectTypeTraits< COrientationValue >
{
    static const TypeIndex::Value Type = TypeIndex::Orientation;
};

typedef CBasicValue< Stretch::Value > CStretchValue;

template< >
struct ObjectTypeTraits< CStretchValue >
{
    static const TypeIndex::Value Type = TypeIndex::Stretch;
};

typedef CBasicValue< StretchDirection::Value > CStretchDirectionValue;

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

        __out const WCHAR* GetValue(
            );

        UINT32 GetLength(
            );

        __override virtual bool Equals(
            __in_opt CObjectWithType* pOther 
            );

        __checkReturn HRESULT Clone( 
            __deref_out CStringValue** ppClone 
            );

    protected:
        CStringValue(
            );

        virtual ~CStringValue(
            );

        __checkReturn HRESULT Initialize( 
            __in_z const WCHAR* pValue 
            );

        __checkReturn HRESULT Initialize(
            __in_ecount(Length) const WCHAR* pValue, 
            UINT32 Length 
            );

        WCHAR* m_Value;
        UINT32 m_Length;
};

template< >
struct ObjectTypeTraits< CStringValue >
{
    static const TypeIndex::Value Type = TypeIndex::String;
};

class CDurationValue : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_FACTORY1( CDurationValue, const CTimeSpan& );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Duration, CObjectWithType );

        __override virtual bool Equals(
            __in_opt CObjectWithType* pOther 
            );

        bool IsForever(
            );

        bool IsAutomatic(
            );

        bool HasTimeSpan(
            );

        const CTimeSpan* GetTimeSpan(
            );

    protected:
        CDurationValue(
            );

        virtual ~CDurationValue(
            );

        __checkReturn HRESULT Initialize( 
            const CTimeSpan& Time 
            );

        CTimeSpan m_TimeSpan;
        bool m_Forever : 1;
        bool m_Automatic : 1;
};

template< >
struct ObjectTypeTraits< CDurationValue >
{
    static const TypeIndex::Value Type = TypeIndex::Duration;
};