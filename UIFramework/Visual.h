#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "Collections.h"
#include "RenderTarget.h"
#include "RenderContext.h"
#include "GraphicsDevice.h"
#include "VisualResource.h"

class CVisual : public CRefCountedObject
{
    typedef std::vector< CVisual* > VisualChildCollection;
    typedef std::vector< CVisualResource* > VisualResourceCollection;

    public:
        virtual HRESULT PreRender( CPreRenderContext& Context );
        virtual HRESULT Render( CRenderContext& Context );

        HRESULT SetVisualTransform( const Matrix3X2& Matrix );
        const Matrix3X2& GetVisualTransform();
    
    protected:
        CVisual();
        virtual ~CVisual();
    
        HRESULT Initialize();
        HRESULT Finalize();
    
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
        Matrix3X2 m_VisualTransform;
        BOOL m_VisualAttached;
};