#pragma once

#include "TextLayoutMetrics.h"
#include "Factory.h"

#include <dwrite.h>

class CDirectWriteLayoutMetrics : public CTextLayoutMetrics
{
    public:
        DECLARE_FACTORY1( CDirectWriteLayoutMetrics, const DWRITE_TEXT_METRICS& );

        virtual HRESULT GetBounds( RectF* pBounds );

    protected:
        CDirectWriteLayoutMetrics();
        virtual ~CDirectWriteLayoutMetrics();

        virtual HRESULT Initialize( const DWRITE_TEXT_METRICS& Metrics );

        DWRITE_TEXT_METRICS m_Metrics;
};
