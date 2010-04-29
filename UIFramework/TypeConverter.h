#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"

class CTypeConverter : public CRefCountedObject
{
    public:
        virtual HRESULT Convert( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue ) = 0;
};
