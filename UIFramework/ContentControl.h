#pragma once

#include "Control.h"

class CContentControl : public CControl
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::ContentControl, CControl );

    protected:
        CContentControl();
        virtual ~CContentControl();
};

template< >
struct ObjectTypeTraits< CContentControl >
{
    static const TypeIndex::Value Type = TypeIndex::ContentControl;
};