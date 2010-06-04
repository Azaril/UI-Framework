#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "Collections.h"
#include "RenderTarget.h"
#include "RenderContext.h"
#include "GraphicsDevice.h"
#include "VisualResource.h"
#include "HitTestResult.h"
#include "Transform.h"

class CVisual : public CRefCountedObjectBase< CPropertyObject >
{
    typedef std::vector< CVisual* > VisualChildCollection;
    typedef std::vector< CVisualResource* > VisualResourceCollection;

    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Visual, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT PreRender( CPreRenderContext& Context );
        virtual HRESULT Render( CRenderContext& Context );

        HRESULT SetVisualTransform( const Matrix3X2F& Matrix );
        const Matrix3X2F& GetVisualTransform();

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult ) = 0;

        virtual UINT32 GetVisualChildCount();
        virtual CVisual* GetVisualChild( UINT32 Index );

        virtual HRESULT TransformToParent( CTransform** ppTransform );

        virtual HRESULT OnVisualNotification( CVisualNotification* pNotification );
    
    protected:
        CVisual();
        virtual ~CVisual();
   
        HRESULT Initialize();
        HRESULT Finalize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
    
        virtual HRESULT OnVisualAttach( CVisualAttachContext& Context );
        virtual HRESULT OnVisualDetach( CVisualDetachContext& Context );

        virtual HRESULT AddChildVisual( CVisual* pVisualChild );
        virtual HRESULT RemoveChildVisual( CVisual* pVisualChild );

        virtual HRESULT MoveToBack( CVisual* pVisualChild );
        virtual HRESULT MoveToFront( CVisual* pVisualChild );

        virtual HRESULT AddVisualResource( CVisualResource* pVisualResource );
        virtual HRESULT RemoveVisualResource( CVisualResource* pVisualResource );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        CVisualAttachContext m_VisualContext;
        VisualChildCollection m_VisualChildren;
        VisualResourceCollection m_VisualResources;
        Matrix3X2F m_VisualTransform;
        Matrix3X2F m_FinalLocalTransform;
        BOOL m_VisualAttached;
};

template< >
struct ObjectTypeTraits< CVisual >
{
    static const TypeIndex::Value Type = TypeIndex::Visual;
};