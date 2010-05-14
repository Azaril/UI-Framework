#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Providers.h"

class CBinding;

typedef signal< void ( CBinding* ) > BindingInvalidatedSignal;
typedef BindingInvalidatedSignal::slot_type BindingInvalidatedHandler;

class CBinding : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Binding, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetBoundValue( CObjectWithType** ppValue ) = 0;

        virtual HRESULT SetTarget( CPropertyObject* pTarget, CProperty* pTargetProperty );
        virtual HRESULT ClearTarget();

        HRESULT AddChangeListener( const BindingInvalidatedHandler& Handler, connection* pConnection );

    protected:
        CBinding();
        virtual ~CBinding();

        HRESULT Initialize( CProviders* pProviders );

        HRESULT InvalidateBinding();

        CProviders* m_Providers;
        CPropertyObject* m_Target;
        CProperty* m_TargetProperty;
        BindingInvalidatedSignal m_InvalidatedSignal;
};

template< >
struct ObjectTypeTraits< CBinding >
{
    static const TypeIndex::Value Type = TypeIndex::Binding;
};