#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "RenderTarget.h"
#include "RootUIElement.h"

class CUIHost : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CUIHost, CGraphicsDevice*, CRenderTarget* );

        virtual HRESULT GetRootElement( CRootUIElement** ppElement );

        HRESULT Render();

    protected:
        CUIHost();
        virtual ~CUIHost();

        HRESULT Initialize( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget );

        HRESULT EnsureLayout();

        CRenderTarget* m_RenderTarget;
        CRootUIElement* m_RootElement;
};
