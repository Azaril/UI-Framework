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

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual __checkReturn HRESULT GetValue( 
            __in CProperty* pProperty, 
            __in CObjectWithType** ppValue 
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
        static CStaticProperty PropertyProperty;

    protected:
        CTemplateBinding(
            );

        virtual ~CTemplateBinding(
            );

        __checkReturn HRESULT Initialize( 
            __in CProviders* pProviders 
            );

        __override virtual __checkReturn HRESULT SetValueInternal(
            __in CProperty* pProperty,
            __in CObjectWithType* pValue 
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
        CStringValue* m_Property;
};

template< >
struct ObjectTypeTraits< CTemplateBinding >
{
    static const TypeIndex::Value Type = TypeIndex::TemplateBinding;
};