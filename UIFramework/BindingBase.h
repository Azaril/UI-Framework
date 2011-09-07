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

        static __checkReturn HRESULT CreatePropertyInformation(
            __deref_out CPropertyInformation** ppInformation 
            );

        virtual __checkReturn HRESULT GetBoundValue( 
            __deref_out_opt CObjectWithType** ppValue 
            ) = 0;

        virtual __checkReturn HRESULT SetTarget( 
            __in CPropertyObject* pTarget, 
            __in CProperty* pTargetProperty 
            );

        virtual __checkReturn HRESULT ClearTarget(
            );

        __out_opt CPropertyObject* GetTarget(
            );

        __out_opt CProperty* GetTargetProperty(
            );

        __checkReturn HRESULT AddChangeListener( 
            const BindingInvalidatedHandler& Handler,
            __out events::signals::connection* pConnection 
            );

    protected:
        CBindingBase(
            );

        virtual ~CBindingBase(
            );

        __checkReturn HRESULT Initialize( 
            __in CProviders* pProviders 
            );

        __checkReturn HRESULT InvalidateBinding(
            );

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