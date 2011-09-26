#pragma once

#include "PropertyObject.h"
#include "Collection.h"

class CResolvedTriggerAction;
class CUIElement;
struct IStyleCallback;

class UIFRAMEWORK_API CTriggerAction : public CPropertyObject
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::TriggerAction, CPropertyObject );

        virtual __checkReturn HRESULT ResolveAction(
            __in CUIElement* pObject,
            __in IStyleCallback* pCallback,
            __deref_out CResolvedTriggerAction** ppResolvedAction
            ) = 0;        
};

template< >
struct ObjectTypeTraits< CTriggerAction >
{
    static const TypeIndex::Value Type = TypeIndex::TriggerAction;
};

class CTriggerActionCollection : public CCollection< CTriggerAction >
{
    public:
        DECLARE_FACTORY( CTriggerActionCollection );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TriggerActionCollection, CCollection< CTriggerAction > );
};

template< >
struct ObjectTypeTraits< CTriggerActionCollection >
{
    static const TypeIndex::Value Type = TypeIndex::TriggerActionCollection;
};

class UIFRAMEWORK_API CResolvedTriggerAction : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT Apply(
            ) = 0;
};