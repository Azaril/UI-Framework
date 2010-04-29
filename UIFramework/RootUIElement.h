#pragma once

#include "FrameworkElement.h"

class CRootUIElement : public CFrameworkElement,
                       public IRenderRoot
{
    public:
        DECLARE_FACTORY2( CRootUIElement, CGraphicsDevice*, CRenderTarget* );

        virtual HRESULT PreRender( CPreRenderContext& Context );

        virtual HRESULT RenderRoot( CRenderContext& Context );

        virtual HRESULT SetChild( CUIElement* pChild );

        virtual HRESULT Arrange( SizeF Size );

    protected:
        CRootUIElement();
        virtual ~CRootUIElement();

        HRESULT Initialize( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget );
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        CUIElement* m_Child;
};
