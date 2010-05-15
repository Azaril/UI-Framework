#pragma once

#include "FrameworkElement.h"

class CRootUIElement : public CFrameworkElement,
                       public IRenderRoot
{
    public:
        DECLARE_FACTORY3( CRootUIElement, CGraphicsDevice*, CRenderTarget*, CProviders* );

        virtual HRESULT RenderRoot( CRenderContext& Context );

        virtual HRESULT SetChild( CUIElement* pChild );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        virtual CNamescope* GetNamescope();

    protected:
        CRootUIElement();
        virtual ~CRootUIElement();

        HRESULT Initialize( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CProviders* pProviders );
        HRESULT Finalize();

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        CUIElement* m_Child;
        CNamescope* m_Namescope;

    private:
        CProviders* m_Providers;
};
