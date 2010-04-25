#pragma once

#include "RefCounted.h"

class CTextLayoutMetrics : public CRefCountedObject
{
    public:
        virtual HRESULT GetBounds( RectF* pBounds ) = 0;
};
