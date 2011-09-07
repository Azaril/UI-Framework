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

        //static __checkReturn HRESULT CreatePropertyInformation( 
        // __deref_out CPropertyInformation** ppInformation 
        // );

        //
        // Properties
        //
        //static CStaticProperty BindingDirectionProperty;

    protected:
        CBinding(
            );

        virtual ~CBinding(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );

        __override virtual __checkReturn HRESULT SetValueInternal( 
            __in CProperty* pProperty, 
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetValueInternal( 
            __in CProperty* pProperty, 
            __deref_out CObjectWithType** ppValue 
            );
};

template< >
struct ObjectTypeTraits< CBinding >
{
    static const TypeIndex::Value Type = TypeIndex::Binding;
};