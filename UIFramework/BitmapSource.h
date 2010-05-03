#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"

class CBitmapSource : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::BitmapSource, CObjectWithType );

        virtual HRESULT GetSize( SizeU* pSize ) = 0;
};

template< >
struct ObjectTypeTraits< CBitmapSource >
{
    static const TypeIndex::Value Type = TypeIndex::BitmapSource;
};