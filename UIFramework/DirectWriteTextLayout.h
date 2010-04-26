#pragma once

#include "TextLayout.h"
#include "Factory.h"

#include <dwrite.h>

class CDirectWriteTextLayout : public CTextLayout
{
    public:
        DECLARE_FACTORY1( CDirectWriteTextLayout, IDWriteTextLayout* );

        IDWriteTextLayout* GetDirectWriteTextLayout();

        virtual HRESULT SetMaxSize( const SizeF& Size );
        virtual HRESULT GetMetrics( CTextLayoutMetrics** ppMetrics );

    protected:
        CDirectWriteTextLayout();
        virtual ~CDirectWriteTextLayout();

        HRESULT Initialize( IDWriteTextLayout* pLayout );

        IDWriteTextLayout* m_Layout;
};
