#pragma once

#include "Binding.h"
#include "Factory.h"
#include "StaticPropertyInformation.h"
#include "RoutedEventArgs.h"

class CDynamicResource : public CBindingBase
{
    public:
        DECLARE_FACTORY1( CDynamicResource, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::DynamicResource, CBindingBase );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual __checkReturn HRESULT GetBoundValue(
            __deref_out_opt CObjectWithType** ppValue 
            );

        __override virtual __checkReturn HRESULT SetTarget(
            __in CPropertyObject* pTarget, 
            __in CProperty* pTargetProperty 
            );

        __override virtual __checkReturn HRESULT ClearTarget(
            );

        //
        // Properties
        //
        static CStaticProperty ResourceKeyProperty;

    protected:
        CDynamicResource(
            );

        virtual ~CDynamicResource(
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
            __deref_out_opt CObjectWithType** ppValue 
            );

        void OnTargetAttached( 
            __in CObjectWithType* pSender, 
            __in CRoutedEventArgs* pRoutedEventArgs 
            );

        void OnTargetDetached(
            __in CObjectWithType* pSender, 
            __in CRoutedEventArgs* pRoutedEventArgs 
            );

        events::signals::connection m_TargetAttachedConnection;
        events::signals::connection m_TargetDetachedConnection;
        CObjectWithType* m_ResourceKey;
};

template< >
struct ObjectTypeTraits< CDynamicResource >
{
    static const TypeIndex::Value Type = TypeIndex::DynamicResource;
};