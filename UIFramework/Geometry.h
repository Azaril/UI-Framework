#pragma once

#include "RefCounted.h"
#include "VisualResource.h"
#include "PropertyObject.h"

class CGeometry : public CObjectWithType
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Geometry, CObjectWithType );

        virtual HRESULT GetBounds( RectF* pBounds ) = 0;
        virtual HRESULT GetBounds( const Matrix3X2F& Transform, RectF* pBounds ) = 0;
        virtual HRESULT FillContainsPoint( const Point2F& LocalPoint, BOOL* pContainsPoint ) = 0;
};

template< >
struct ObjectTypeTraits< CGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::Geometry;
};

class CRectangleGeometry : public CGeometry
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RectangleGeometry, CGeometry );
};

template< >
struct ObjectTypeTraits< CRectangleGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::RectangleGeometry;
};

class CRoundedRectangleGeometry : public CGeometry
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RoundedRectangleGeometry, CGeometry );
};

template< >
struct ObjectTypeTraits< CRoundedRectangleGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::RoundedRectangleGeometry;
};