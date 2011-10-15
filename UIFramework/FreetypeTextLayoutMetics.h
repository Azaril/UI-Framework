#pragma once

#include "TextLayoutMetrics.h"
#include "Factory.h"

class CFreetypeTextLayoutMetics : public CTextLayoutMetrics
{
    public:
        DECLARE_FACTORY1( CFreetypeTextLayoutMetics, const RectF& );

        __override virtual __checkReturn HRESULT GetBounds(
            __out RectF* pBounds
            );

    protected:
        CFreetypeTextLayoutMetics(
            );

        virtual ~CFreetypeTextLayoutMetics(
            );

        __checkReturn HRESULT Initialize(
            const RectF& bounds
            );

        RectF m_Bounds;
};

