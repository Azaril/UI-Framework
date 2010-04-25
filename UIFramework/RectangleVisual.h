#pragma once

#include "Visual.h"

class CRectangleVisual : public CVisual
{
    public:
        DECLARE_FACTORY( CRectangleVisual );

        HRESULT SetSize( SizeF Size );
        HRESULT SetPosition( Point2F Position );
        HRESULT SetBrush( CBrush* pBrush );

    protected:
        CRectangleVisual();
        virtual ~CRectangleVisual();

        virtual HRESULT Initialize();

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        SizeF m_Size;
        Point2F m_Position;
        CBrush* m_Brush;
};
