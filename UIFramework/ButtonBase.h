#pragma once

#include "ContentControl.h"

class CButtonBase : public CContentControl
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::ButtonBase, CContentControl );
        
    protected:
        CButtonBase();
        virtual ~CButtonBase();
};

template< >
struct ObjectTypeTraits< CButtonBase >
{
    static const TypeIndex::Value Type = TypeIndex::ButtonBase;
};
