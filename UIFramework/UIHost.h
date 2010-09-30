#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "RenderTarget.h"
#include "RootUIElement.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "FocusManager.h"

class UIFRAMEWORK_API CUIHost : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CUIHost, CGraphicsDevice*, CRenderTarget*, CProviders* );

        virtual HRESULT GetRootElement( CRootUIElement** ppElement );

        HRESULT Render();

        HRESULT GetMouseController( CMouseController** ppController );
        HRESULT GetKeyboardController( CKeyboardController** ppController );

        HRESULT GetFocusManager( CFocusManager** ppFocusManager );

    protected:
        CUIHost();
        virtual ~CUIHost();

        HRESULT Initialize( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CProviders* pProviders );

        HRESULT EnsureLayout();

        CRenderTarget* m_RenderTarget;
        CRootUIElement* m_RootElement;
        SizeF m_LastLayoutSize;
        CFocusManager* m_FocusManager;
        CMouseController* m_MouseController;
        CKeyboardController* m_KeyboardController;
};
