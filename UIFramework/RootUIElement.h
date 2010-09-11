#pragma once

#include "FrameworkElement.h"

class UIFRAMEWORK_API CRootUIElement : public CFrameworkElement,
                       public IRenderRoot
{
    public:
        DECLARE_FACTORY4( CRootUIElement, CGraphicsDevice*, CRenderTarget*, CProviders*, CFocusManager* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Image, CFrameworkElement );

        virtual HRESULT RenderRoot( CRenderContext& Context );

        virtual HRESULT SetChild( CUIElement* pChild );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CRootUIElement();
        virtual ~CRootUIElement();

        HRESULT Initialize( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CProviders* pProviders, CFocusManager* pFocusManager );
        HRESULT Finalize();

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        CUIElement* m_Child;
        CNamescope* m_Namescope;

    private:
        CProviders* m_Providers;
};
