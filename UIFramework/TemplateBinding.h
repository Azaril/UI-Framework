#pragma once

#include "Binding.h"
#include "Factory.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "RoutedEventArgs.h"
#include "SourcedBinding.h"

class CTemplateBinding : public CSourcedBinding
{
    public:
        DECLARE_FACTORY1( CTemplateBinding, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TemplateBinding, CSourcedBinding );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT SetTarget( CPropertyObject* pTarget, CProperty* pTargetProperty );
        virtual HRESULT ClearTarget();

        //
        // Properties
        //
        static CStaticProperty PropertyProperty;

    protected:
        CTemplateBinding();
        virtual ~CTemplateBinding();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        void OnTargetAttached( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        void OnTargetDetached( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        events::signals::connection m_TargetAttachedConnection;
        events::signals::connection m_TargetDetachedConnection;
        CStringValue* m_Property;
};

template< >
struct ObjectTypeTraits< CTemplateBinding >
{
    static const TypeIndex::Value Type = TypeIndex::TemplateBinding;
};