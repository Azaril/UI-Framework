#pragma once

#include "Visual.h"
#include "Brush.h"
#include "Providers.h"

class UIFRAMEWORK_API CTextVisual : public CVisual
{
    public:
        DECLARE_FACTORY( CTextVisual );
        DECLARE_FACTORY1( CTextVisual, CTextLayout* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TextVisual, CVisual );

        __checkReturn HRESULT SetForegroundBrush(
            __in_opt CBrush* pBrush 
            );

        __checkReturn HRESULT SetTextLayout(
            __in_opt CTextLayout* pTextLayout 
            );

        virtual __checkReturn HRESULT PreRender(
            CPreRenderContext& Context 
            );

        virtual __checkReturn HRESULT HitTest( 
            Point2F LocalPoint, 
            __deref_out_opt CHitTestResult** ppHitTestResult 
            );

    protected:
        CTextVisual(
            );

        virtual ~CTextVisual(
            );

        __checkReturn HRESULT Initialize(
            __in_opt CTextLayout* pTextLayout = NULL 
            );

        __checkReturn HRESULT SetForegroundBrushInternal( 
            __in_opt CBrush* pBrush 
            );

        virtual __checkReturn HRESULT RenderTransformed( 
            CRenderContext& Context 
            );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnForegroundBrushChanged );

        __checkReturn HRESULT OnForegroundBrushChanged(
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );

        CBrush* m_ForegroundBrush;
        CGraphicsBrush* m_ForegroundGraphicsBrush;
        CTextLayout* m_TextLayout;
        Matrix3X2F m_ForegroundBrushTransform;
        BOOL m_UpdateBrushTransform;
};

template< >
struct ObjectTypeTraits< CTextVisual >
{
    static const TypeIndex::Value Type = TypeIndex::TextVisual;
};