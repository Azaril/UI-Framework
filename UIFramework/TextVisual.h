#pragma once

#include "Visual.h"
#include "Brush.h"
#include "Providers.h"

class UIFRAMEWORK_API CTextVisual : public CVisual
{
    public:
        DECLARE_FACTORY( CTextVisual );
        DECLARE_FACTORY1( CTextVisual, CTextLayout* );

        HRESULT SetForegroundBrush( CBrush* pBrush );
        HRESULT SetTextLayout( CTextLayout* pTextLayout );

        virtual HRESULT PreRender( CPreRenderContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CTextVisual();
        virtual ~CTextVisual();

        HRESULT Initialize( CTextLayout* pTextLayout = NULL );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        CBrush* m_ForegroundBrush;
        CGraphicsBrush* m_ForegroundGraphicsBrush;
        CTextLayout* m_TextLayout;
        CTextVisual* m_TextVisual;
};
