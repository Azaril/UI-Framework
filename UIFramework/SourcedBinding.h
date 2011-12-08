#pragma once

#include "BindingBase.h"
#include "StaticPropertyInformation.h"

namespace BindingDirection
{
    enum Value
    {
        OneWay,
        TwoWay
    };
}

typedef CBasicValue< BindingDirection::Value > CBindingDirectionValue;

template< >
struct ObjectTypeTraits< CBindingDirectionValue >
{
    static const TypeIndex::Value Type = TypeIndex::BindingDirection;
};

class CSourcedBinding : public CBindingBase
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::SourcedBinding, CBindingBase );

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
        static CStaticProperty BindingDirectionProperty;

    protected:
        CSourcedBinding(
            );

        virtual ~CSourcedBinding(
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

        __checkReturn HRESULT SetSource(
            __in CPropertyObject* pSource, 
            __in CProperty* pSourceProperty 
            );

        __checkReturn HRESULT ClearSource(
            );

        void OnSourcePropertyChanged(
            __in CPropertyObject* pObject,
            __in CProperty* pProperty 
            );

        void OnTargetPropertyChanged( 
            __in CPropertyObject* pObject, 
            __in CProperty* pProperty 
            );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnBindingDirectionChanged );

        __checkReturn HRESULT OnBindingDirectionChanged( 
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );

    private:
        CPropertyObject* m_Source;
        CProperty* m_SourceProperty;
        BindingDirection::Value m_BindingDirection;
        events::signals::connection m_SourcePropertyChangedConnection;
        events::signals::connection m_TargetPropertyChangedConnection;
};

template< >
struct ObjectTypeTraits< CSourcedBinding >
{
    static const TypeIndex::Value Type = TypeIndex::SourcedBinding;
};