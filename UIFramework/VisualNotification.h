#pragma once

#include "RefCounted.h"

namespace VisualNotification
{
    enum Value
    {
        ChildBrushInvalidated
    };
}

class CVisualNotification : public CRefCountedObject
{
    public:
        virtual VisualNotification::Value GetType() = 0;
};