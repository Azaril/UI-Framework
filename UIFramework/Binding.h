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

        //static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        //
        // Properties
        //
        //static CStaticProperty BindingDirectionProperty;

    protected:
        CBinding();
        virtual ~CBinding();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );
};

template< >
struct ObjectTypeTraits< CBinding >
{
    static const TypeIndex::Value Type = TypeIndex::Binding;
};