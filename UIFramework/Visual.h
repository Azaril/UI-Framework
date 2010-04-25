#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "Visitor.h"
#include "Collections.h"
#include "RenderTarget.h"
#include "MatrixStack.h"
#include "GraphicsDevice.h"

class CVisualAttachContext 
{
    public:
        CVisualAttachContext() : m_GraphicsDevice(NULL)
        {
        }

        CVisualAttachContext( CGraphicsDevice* pGraphicsDevice ) : m_GraphicsDevice(pGraphicsDevice)
        {
        }

        CGraphicsDevice* GetGraphicsDevice()
        {
            return m_GraphicsDevice;
        }

        void Reset()
        {
            m_GraphicsDevice = NULL;
        }

    protected:
        CGraphicsDevice* m_GraphicsDevice;
};

class CVisualDetachContext 
{
    public:
        CVisualDetachContext()
        {
        }
};

class CRenderContext
{
    public:
        CRenderContext( CRenderTarget* pRenderTarget ) : m_RenderTarget(pRenderTarget)
        {
        }

        CRenderTarget* GetRenderTarget()
        {
            return m_RenderTarget;
        }

        CMatrixStack& GetMatrixStack()
        {
            return m_MatrixStack;
        }

    protected:
        CMatrixStack m_MatrixStack;
        CRenderTarget* m_RenderTarget;
};

struct IRenderRoot
{
    virtual HRESULT RenderRoot( CRenderContext& Context ) = 0;
};

class CPreRenderContext
{
    typedef std::vector< IRenderRoot* > RenderRootCollection;

    public:
        CPreRenderContext( CRenderTarget* pRenderTarget ) : m_RenderTarget(pRenderTarget)
        {
        }

        CRenderTarget* GetRenderTarget()
        {
            return m_RenderTarget;
        }

        HRESULT AddRenderRoot( IRenderRoot* pRoot )
        {
            HRESULT hr = S_OK;

            IFCPTR(pRoot);

            m_RenderRoots.push_back(pRoot);

        Cleanup:
            return hr;
        }

        HRESULT RenderRoots( CRenderContext& Context )
        {
            HRESULT hr = S_OK;

            for(RenderRootCollection::reverse_iterator It = m_RenderRoots.rbegin(); It != m_RenderRoots.rend(); ++It)
            {
                IFC((*It)->RenderRoot(Context));
            }

        Cleanup:
            return hr;
        }

    protected:
        CRenderTarget* m_RenderTarget;
        RenderRootCollection m_RenderRoots;
};

class CVisual : public CRefCountedObject
{
    typedef std::vector< CVisual* > VisualChildCollection;

    public:
        DECLARE_VISITABLE();

        virtual HRESULT PreRender( CPreRenderContext& Context );
        virtual HRESULT Render( CRenderContext& Context );

        HRESULT SetVisualTransform( const Matrix3X2& Matrix );
        const Matrix3X2& GetVisualTransform();
    
    protected:
        CVisual();
        virtual ~CVisual();
    
        HRESULT Initialize();
    
        virtual HRESULT OnVisualAttach( CVisualAttachContext& Context );
        virtual HRESULT OnVisualDetach( CVisualDetachContext& Context );

        virtual HRESULT AddChildVisual( CVisual* pVisualChild );
        virtual HRESULT RemoveChildVisual( CVisual* pVisualChild );

        virtual HRESULT MoveToBack( CVisual* pVisualChild );
        virtual HRESULT MoveToFront( CVisual* pVisualChild );

        virtual HRESULT RenderTransformed( CRenderContext& Context );

        CVisualAttachContext m_VisualContext;
        VisualChildCollection m_VisualChildren;
        Matrix3X2 m_VisualTransform;
        BOOL m_VisualAttached;
};