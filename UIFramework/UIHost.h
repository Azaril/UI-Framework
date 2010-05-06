#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "RenderTarget.h"
#include "RootUIElement.h"

class CUIHost : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CUIHost, CGraphicsDevice*, CRenderTarget*, CProviders* );

        virtual HRESULT GetRootElement( CRootUIElement** ppElement );

        HRESULT Render();

        HRESULT InjectMouseButton( MouseButton::Value Button, MouseButtonState::Value State, Point2F Point );
        HRESULT InjectMouseMove( Point2F Point );

    protected:
        CUIHost();
        virtual ~CUIHost();

        HRESULT Initialize( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CProviders* pProviders );

        HRESULT EnsureLayout();

        CRenderTarget* m_RenderTarget;
        CRootUIElement* m_RootElement;
        SizeF m_LastLayoutSize;
};
