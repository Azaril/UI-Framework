#pragma once

#include "TextLayoutMetrics.h"
#include "Factory.h"

class CTextLayoutEngineMetrics : public CTextLayoutMetrics
{
    public:
        DECLARE_FACTORY1( CTextLayoutEngineMetrics, const RectF& );

        __override virtual __checkReturn HRESULT GetBounds(
            __out RectF* pBounds
            );

    protected:
        CTextLayoutEngineMetrics(
            );

        virtual ~CTextLayoutEngineMetrics(
            );

        __checkReturn HRESULT Initialize(
            const RectF& bounds
            );

        RectF m_Bounds;
};

