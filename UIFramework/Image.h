#pragma once

#include "FrameworkElement.h"
#include "ImageBrush.h"

class CImage : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CImage );

        virtual TypeIndex::Value GetType() { return TypeIndex::Image; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Image || CFrameworkElement::IsTypeOf(Type); }

        virtual HRESULT Arrange( SizeF Size );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );

        HRESULT SetSource( CObjectWithType* pSource );

    protected:
        CImage();
        virtual ~CImage();

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        HRESULT InternalSetSource( CObjectWithType* pSource );

        CObjectWithType* m_Source;
        CImageBrush* m_ImageBrush;
        CRectangleVisual* m_ImageRect;
};
