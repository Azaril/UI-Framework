#pragma once

#include "Visual.h"

class CRectangleVisual : public CVisual
{
    public:
        DECLARE_FACTORY( CRectangleVisual );

        HRESULT SetSize( SizeF Size );
        HRESULT SetPosition( Point2F Position );
        HRESULT SetBorderThickness( RectF Thickness );
        
        HRESULT SetBrush( CBrush* pBrush );
        HRESULT SetOutlineBrush( CBrush* pBrush );

    protected:
        CRectangleVisual();
        virtual ~CRectangleVisual();

        HRESULT Initialize();

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        SizeF m_Size;
        Point2F m_Position;
        RectF m_BorderThickness;
        CBrush* m_Brush;
        CBrush* m_OutlineBrush;
};
