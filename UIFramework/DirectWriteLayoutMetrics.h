#pragma once

#if defined(FRAMEWORK_DWRITE)

#include "TextLayoutMetrics.h"
#include "Factory.h"

#include <dwrite.h>

class CDirectWriteLayoutMetrics : public CTextLayoutMetrics
{
    public:
        DECLARE_FACTORY1( CDirectWriteLayoutMetrics, const DWRITE_TEXT_METRICS& );

        __override virtual __checkReturn HRESULT GetBounds( 
			__out RectF* pBounds
			);

    protected:
        CDirectWriteLayoutMetrics(
			);

        virtual ~CDirectWriteLayoutMetrics(
			);

        __checkReturn HRESULT Initialize( 
			const DWRITE_TEXT_METRICS& Metrics 
			);

        DWRITE_TEXT_METRICS m_Metrics;
};

#endif