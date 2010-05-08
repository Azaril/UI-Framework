#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"

class CBinding : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Binding, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetBoundValue( CObjectWithType* pTarget, CProperty* pTargetProperty, CObjectWithType** ppValue ) = 0;

    protected:
        CBinding();
        virtual ~CBinding();
};

template< >
struct ObjectTypeTraits< CBinding >
{
    static const TypeIndex::Value Type = TypeIndex::Binding;
};