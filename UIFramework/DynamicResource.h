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

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetBoundValue( CObjectWithType** ppValue );

        virtual HRESULT SetTarget( CPropertyObject* pTarget, CProperty* pTargetProperty );
        virtual HRESULT ClearTarget();

        //
        // Properties
        //
        static CStaticProperty ResourceKeyProperty;

    protected:
        CDynamicResource();
        virtual ~CDynamicResource();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );

        void OnTargetAttached( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        void OnTargetDetached( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        events::signals::connection m_TargetAttachedConnection;
        events::signals::connection m_TargetDetachedConnection;
        CObjectWithType* m_ResourceKey;
};

template< >
struct ObjectTypeTraits< CDynamicResource >
{
    static const TypeIndex::Value Type = TypeIndex::DynamicResource;
};