#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Providers.h"
#include "BasicTypes.h"

class CBindingBase;

typedef events::signal< void ( CBindingBase* ) > BindingInvalidatedSignal;
typedef BindingInvalidatedSignal::slot_type BindingInvalidatedHandler;

class CBindingBase : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::BindingBase, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetBoundValue( CObjectWithType** ppValue ) = 0;

        virtual HRESULT SetTarget( CPropertyObject* pTarget, CProperty* pTargetProperty );
        virtual HRESULT ClearTarget();

        CPropertyObject* GetTarget();
        CProperty* GetTargetProperty();

        HRESULT AddChangeListener( const BindingInvalidatedHandler& Handler, events::signals::connection* pConnection );

    protected:
        CBindingBase();
        virtual ~CBindingBase();

        HRESULT Initialize( CProviders* pProviders );

        HRESULT InvalidateBinding();

        CProviders* m_Providers;

    private:
        CPropertyObject* m_Target;
        CProperty* m_TargetProperty;
        BindingInvalidatedSignal m_InvalidatedSignal;
};

template< >
struct ObjectTypeTraits< CBindingBase >
{
    static const TypeIndex::Value Type = TypeIndex::BindingBase;
};