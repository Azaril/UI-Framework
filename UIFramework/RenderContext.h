#pragma once

#include "MatrixStack.h"
#include "Collections.h"

class CRenderTarget;

class CRenderContext
{
    public:
        CRenderContext( CRenderTarget* pRenderTarget );
        ~CRenderContext();

        CRenderTarget* GetRenderTarget();

        CMatrixStack& GetMatrixStack();

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
    typedef vector< IRenderRoot* > RenderRootCollection;

    public:
        CPreRenderContext( CRenderTarget* pRenderTarget );
        ~CPreRenderContext();

        CRenderTarget* GetRenderTarget();

        HRESULT AddRenderRoot( IRenderRoot* pRoot );

        HRESULT RenderRoots( CRenderContext& Context );

    protected:
        CRenderTarget* m_RenderTarget;
        RenderRootCollection m_RenderRoots;
};