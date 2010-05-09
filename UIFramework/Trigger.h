#pragma once

#include "PropertyObject.h"
#include "Collection.h"
#include "Providers.h"
#include "StyleCallback.h"

class CUIElement;

class CResolvedTrigger : public CRefCountedObject
{
    public:
};

class CTrigger : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Trigger, CPropertyObject );

        virtual HRESULT ResolveTrigger( CUIElement* pObject, CProviders* pProviders, IStyleCallback* pCallback, CResolvedTrigger** ppResolvedTrigger ) = 0;

    protected:
};

template< >
struct ObjectTypeTraits< CTrigger >
{
    static const TypeIndex::Value Type = TypeIndex::Trigger;
};

class CTriggerCollection : public CCollection< CTrigger >
{
    public:
        DECLARE_FACTORY( CTriggerCollection );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TriggerCollection, CCollection< CTrigger > );
};

template< >
struct ObjectTypeTraits< CTriggerCollection >
{
    static const TypeIndex::Value Type = TypeIndex::TriggerCollection;
};