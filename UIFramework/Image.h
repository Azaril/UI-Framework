#pragma once

#include "FrameworkElement.h"
#include "ImageBrush.h"
#include "GeometryVisual.h"

class CImage : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CImage );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Image, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );

        HRESULT SetSource( CObjectWithType* pSource );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

    protected:
        CImage();
        virtual ~CImage();

        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        HRESULT InternalSetSource( CObjectWithType* pSource );

        HRESULT RebuildGeometry();
        HRESULT ReleaseGeometry();

        CObjectWithType* m_Source;
        CImageBrush* m_ImageBrush;
        CGeometryVisual* m_ImageVisual;
        BOOL m_GeometryDirty;
};

template< >
struct ObjectTypeTraits< CImage >
{
    static const TypeIndex::Value Type = TypeIndex::Image;
};