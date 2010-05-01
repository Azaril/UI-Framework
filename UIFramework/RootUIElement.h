#pragma once

#include "FrameworkElement.h"

class CRootUIElement : public CFrameworkElement,
                       public IRenderRoot
{
    public:
        DECLARE_FACTORY2( CRootUIElement, CGraphicsDevice*, CRenderTarget* );

        virtual HRESULT RenderRoot( CRenderContext& Context );

        virtual HRESULT SetChild( CUIElement* pChild );

    protected:
        CRootUIElement();
        virtual ~CRootUIElement();

        HRESULT Initialize( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget );
        HRESULT Finalize();

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        CUIElement* m_Child;
};
