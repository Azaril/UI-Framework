#pragma once

#include "FrameworkElement.h"
#include "ImageBrush.h"
#include "GeometryVisual.h"

class UIFRAMEWORK_API CImage : public CFrameworkElement
{
    public:
        DECLARE_FACTORY1( CImage, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Image, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        HRESULT SetSource( const WCHAR* pSource );
        HRESULT SetSource( CObjectWithType* pSource);

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        //
        // Properties
        //
        static CStaticProperty SourceProperty;
        static CStaticProperty StretchProperty;
        static CStaticProperty StretchDirectionProperty;

    protected:
        CImage();
        virtual ~CImage();

        HRESULT Initialize( CProviders* pProviders );
        HRESULT Finalize();

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            const SizeF& AvailableSize,
            SizeF& UsedSize 
            );

        virtual HRESULT PreRenderInternal( CPreRenderContext& Context );

        HRESULT RebuildGeometry();
        HRESULT ReleaseGeometry();

        HRESULT ComputeSize( SizeF AvailableSize, SizeF& UsedSize );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnSourceChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnStretchChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnStretchDirectionChanged );

        HRESULT OnSourceChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnStretchChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnStretchDirectionChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetEffectiveSource( CObjectWithType** ppSource );
        HRESULT GetEffectiveStretch( Stretch::Value* pStretch );
        HRESULT GetEffectiveStretchDirection( StretchDirection::Value* pStretchDirection );

        CTypedLayeredValue< CImage, CObjectWithType > m_Source;
        CTypedLayeredValue< CImage, CStretchValue > m_Stretch;
        CTypedLayeredValue< CImage, CStretchDirectionValue > m_StretchDirection;
        CImageBrush* m_ImageBrush;
        CGeometryVisual* m_ImageVisual;
        bool m_GeometryDirty;
};

template< >
struct ObjectTypeTraits< CImage >
{
    static const TypeIndex::Value Type = TypeIndex::Image;
};