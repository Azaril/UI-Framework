#pragma once

#include "FrameworkElement.h"

class CImage : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CImage );

        virtual HRESULT PreRender( CPreRenderContext& Context );

        virtual HRESULT Arrange( SizeF Size );

        HRESULT SetSource( CBitmapSource* pBitmapSource );

    protected:
        CImage();
        virtual ~CImage();

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        CBitmapSource* m_Source;
        CRectangleVisual* m_ImageRect;
        CBitmap* m_Bitmap;
};
