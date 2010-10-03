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

typedef CBasicValue< BindingDirection::Value, TypeIndex::BindingDirection > CBindingDirectionValue;

template< >
struct ObjectTypeTraits< CBindingDirectionValue >
{
    static const TypeIndex::Value Type = TypeIndex::BindingDirection;
};

class CSourcedBinding : public CBindingBase
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::SourcedBinding, CBindingBase );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetBoundValue( CObjectWithType** ppValue );

        virtual HRESULT SetTarget( CPropertyObject* pTarget, CProperty* pTargetProperty );
        virtual HRESULT ClearTarget();

        //
        // Properties
        //
        static CStaticProperty BindingDirectionProperty;

    protected:
        CSourcedBinding();
        virtual ~CSourcedBinding();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );

        HRESULT SetSource( CPropertyObject* pSource, CProperty* pSourceProperty );
        HRESULT ClearSource();

        void OnSourcePropertyChanged( CPropertyObject* pObject, CProperty* pProperty );
        void OnTargetPropertyChanged( CPropertyObject* pObject, CProperty* pProperty );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnBindingDirectionChanged );

        HRESULT OnBindingDirectionChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

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