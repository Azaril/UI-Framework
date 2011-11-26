#pragma once

#include "RefCounted.h"
#include "VisualResource.h"
#include "PropertyObject.h"

class UIFRAMEWORK_API CGraphicsGeometry : public CObjectWithType
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::GraphicsGeometry, CObjectWithType );

        virtual __checkReturn HRESULT GetBounds(
			__out RectF* pBounds 
			) = 0;

        virtual __checkReturn HRESULT GetBounds( 
			const Matrix3X2F& Transform,
			__out RectF* pBounds 
			) = 0;

        virtual __checkReturn HRESULT FillContainsPoint( 
			const Point2F& LocalPoint, 
			__out bool* pContainsPoint 
			) = 0;
};

template< >
struct ObjectTypeTraits< CGraphicsGeometry >
{
    static const TypeIndex::Value Type = TypeIndex::GraphicsGeometry;
};