#pragma once

#include "FrameworkElement.h"
#include "FontDescription.h"

class UIFRAMEWORK_API CRootUIElement : public CFrameworkElement,
                                       public IRenderRoot
{
    public:
        DECLARE_FACTORY1( CRootUIElement, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::RootUIElement, CFrameworkElement );

        __checkReturn HRESULT SetContext(
            __in CGraphicsDevice* pGraphicsDevice, 
            __in CRenderTarget* pRenderTarget, 
            __in CTimeSource* pTimeSource, 
            __in const CFontDescription* pDefaultFont,
            __in CStaticTreeData* pTreeData 
            );

        __override virtual __checkReturn HRESULT RenderRoot( 
            CRenderContext& Context 
            );

        __checkReturn HRESULT SetChild(
            __in_opt CUIElement* pChild 
            );

        __override virtual __checkReturn HRESULT HitTest( 
            Point2F LocalPoint, 
            __deref_out_opt CHitTestResult** ppHitTestResult 
            );

        __override virtual __out_opt const CFontDescription* GetEffectiveFontDescription(
            );

    protected:
        CRootUIElement(
            );

        virtual ~CRootUIElement(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );

        __checkReturn HRESULT Finalize(
            );

        __override virtual __checkReturn HRESULT PreRenderInternal( 
            CPreRenderContext& Context 
            );

        __override virtual __checkReturn HRESULT MeasureInternal(
            SizeF AvailableSize, 
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            SizeF AvailableSize, 
            SizeF& UsedSize 
            );

        CUIElement* m_Child;
        CNamescope* m_Namescope;
        CFontDescription m_FontDescription;
};
