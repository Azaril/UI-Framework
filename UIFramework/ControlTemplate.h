#pragma once

#include "PropertyObject.h"

class CControlTemplate : public CPropertyObject
{
    public:

    protected:
        CControlTemplate();
        virtual ~CControlTemplate();
};

template< >
struct ObjectTypeTraits< CControlTemplate >
{
    static const TypeIndex::Value Type = TypeIndex::Control;
};