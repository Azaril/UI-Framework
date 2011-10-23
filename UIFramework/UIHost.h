#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "RenderTarget.h"
#include "RootUIElement.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "FocusManager.h"
#include "TimeController.h"
#include "LayoutManager.h"

class UIFRAMEWORK_API CUIHost : public CRefCountedObject
{
    public:
        DECLARE_FACTORY4( CUIHost, CGraphicsDevice*, CRenderTarget*, CProviders*, const CFontDescription* );

        __checkReturn HRESULT GetRootElement( 
            __deref_out CRootUIElement** ppElement 
            );

        __checkReturn HRESULT Render(
            );

        __checkReturn HRESULT GetMouseController(
            __deref_out CMouseController** ppController 
            );

        __checkReturn HRESULT GetKeyboardController( 
            __deref_out CKeyboardController** ppController 
            );

        __checkReturn HRESULT GetTimeController( 
            __deref_out CTimeController** ppController 
            );

        __checkReturn HRESULT GetFocusManager( 
            __deref_out CFocusManager** ppFocusManager
            );

    protected:
        CUIHost(
            );

        virtual ~CUIHost(
            );

        __checkReturn HRESULT Initialize(
            __in CGraphicsDevice* pGraphicsDevice,
            __in CRenderTarget* pRenderTarget,
            __in CProviders* pProviders, 
            __in const CFontDescription* pDefaultFont 
            );

        CTime m_Time;
        CRenderTarget* m_RenderTarget;
        CRootUIElement* m_RootElement;
        CFocusManager* m_FocusManager;
        CMouseController* m_MouseController;
        CKeyboardController* m_KeyboardController;
        CTimeController* m_TimeController;
        CStaticTreeData* m_TreeData;
        CLayoutManager* m_LayoutManager;
};
