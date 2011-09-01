#pragma once

#include "RefCounted.h"
#include "VisualResource.h"
#include "PropertyObject.h"

class UIFRAMEWORK_API CGeometry : public CObjectWithType
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Geometry, CObjectWithType );

        virtual __checkReturn HRESULT GetBounds(
			__out RectF* pBounds 
			) = 0;

        virtual __checkReturn HRESULT GetBounds( 
			const Matrix3X2F& Transform,
			__out RectF* pBounds 
			) = 0;

        virtual __checkReturn HRESULT FillContainsPoint( 
			const Point2F& LocalPoint, 
			__out BOOL* pContainsPoint 
			) = 0;
};

template< >
struct ObjectTypeTraits< CGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::Geometry;
};

class UIFRAMEWORK_API CRectangleGeometry : public CGeometry
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RectangleGeometry, CGeometry );
};

template< >
struct ObjectTypeTraits< CRectangleGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::RectangleGeometry;
};

class UIFRAMEWORK_API CRoundedRectangleGeometry : public CGeometry
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::RoundedRectangleGeometry, CGeometry );
};

template< >
struct ObjectTypeTraits< CRoundedRectangleGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::RoundedRectangleGeometry;
};