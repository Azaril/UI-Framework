#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Providers.h"
#include "BasicTypes.h"
#include "StaticPropertyInformation.h"
#include "SourcedBinding.h"

class CBinding : public CSourcedBinding
{
    public:
        DECLARE_FACTORY1( CBinding, CProviders* )

        DECLARE_TYPE_WITH_BASE( TypeIndex::Binding, CSourcedBinding );

    protected:
        CBinding(
            );

        virtual ~CBinding(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );
};

template< >
struct ObjectTypeTraits< CBinding >
{
    static const TypeIndex::Value Type = TypeIndex::Binding;
};