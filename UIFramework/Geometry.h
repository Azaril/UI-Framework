#pragma once

#include "RefCounted.h"
#include "VisualResource.h"
#include "PropertyObject.h"

class CGeometry : public CObjectWithType
{
    public:
        virtual TypeIndex::Value GetType() { return TypeIndex::Geometry; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Geometry || CObjectWithType::IsTypeOf(Type); }

        virtual HRESULT FillContainsPoint( const Point2F& LocalPoint, BOOL* pContainsPoint ) = 0;
};

class CRectangleGeometry : public CGeometry
{
    public:
        virtual TypeIndex::Value GetType() { return TypeIndex::RectangleGeometry; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::RectangleGeometry || CGeometry::IsTypeOf(Type); }
};