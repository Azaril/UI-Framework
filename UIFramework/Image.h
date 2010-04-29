#pragma once

#include "FrameworkElement.h"

class CImage : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CImage );

        virtual TypeIndex::Value GetType() { return TypeIndex::Image; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Image || CFrameworkElement::IsTypeOf(Type); }

        virtual HRESULT PreRender( CPreRenderContext& Context );

        virtual HRESULT Arrange( SizeF Size );

        HRESULT SetSource( CBitmapSource* pBitmapSource );

    protected:
        CImage();
        virtual ~CImage();

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        CBitmapSource* m_Source;
        CRectangleVisual* m_ImageRect;
        CBitmap* m_Bitmap;
};
