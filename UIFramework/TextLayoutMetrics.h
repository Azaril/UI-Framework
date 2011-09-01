#pragma once

#include "RefCounted.h"

class CTextLayoutMetrics : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT GetBounds(
			__out RectF* pBounds
			) = 0;
};
