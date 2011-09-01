#pragma once

#include "TextLayout.h"
#include "Factory.h"

#include <dwrite.h>

class CDirectWriteTextLayout : public CTextLayout
{
    public:
        DECLARE_FACTORY1( CDirectWriteTextLayout, IDWriteTextLayout* );

        __checkReturn HRESULT GetDirectWriteTextLayout( 
			__deref_out IDWriteTextLayout** ppLayout
			);

        __override virtual __checkReturn HRESULT SetMaxSize(
			const SizeF& Size 
			);

        __override virtual __checkReturn HRESULT GetMetrics(
			__deref_out CTextLayoutMetrics** ppMetrics 
			);

    protected:
        CDirectWriteTextLayout(
			);

        virtual ~CDirectWriteTextLayout(
			);

        __checkReturn HRESULT Initialize(
			__in IDWriteTextLayout* pLayout 
			);

        IDWriteTextLayout* m_Layout;
};
