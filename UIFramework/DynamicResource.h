#pragma once

#include "Binding.h"
#include "Factory.h"
#include "StaticPropertyInformation.h"

class CDynamicResource : public CBinding
{
    public:
        DECLARE_FACTORY( CDynamicResource );

        DECLARE_TYPE_WITH_BASE( TypeIndex::DynamicResource, CBinding );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT GetBoundValue( CObjectWithType* pTarget, CProperty* pTargetProperty, CObjectWithType** ppValue );

        //
        // Properties
        //
        static CStaticProperty ResourceKeyProperty;

    protected:
        CDynamicResource();
        virtual ~CDynamicResource();

        HRESULT Initialize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        CObjectWithType* m_ResourceKey;
};

template< >
struct ObjectTypeTraits< CDynamicResource >
{
    static const TypeIndex::Value Type = TypeIndex::DynamicResource;
};