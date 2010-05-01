#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"

class CBitmapSource : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        virtual TypeIndex::Value GetType() { return TypeIndex::BitmapSource; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::BitmapSource || CObjectWithType::IsTypeOf(Type); }

        virtual HRESULT GetSize( SizeU* pSize ) = 0;
};
