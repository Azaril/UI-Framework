#pragma once

#include "PropertyObject.h"
#include "Providers.h"

class CMarkupExtension : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::MarkupExtension, CPropertyObject );

        virtual HRESULT ExecuteMarkup( CObjectWithType** ppObject ) = 0;

    protected:
};

template< >
struct ObjectTypeTraits< CMarkupExtension >
{
    static const TypeIndex::Value Type = TypeIndex::MarkupExtension;
};