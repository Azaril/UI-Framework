#pragma once

#include "RefCounted.h"
#include "UIElement.h"
#include "MouseInput.h"
#include "FocusManager.h"

class UIFRAMEWORK_API CKeyboardController :  public CRefCountedObject 
{
    public:
        DECLARE_FACTORY2( CKeyboardController, CFocusManager*, CUIElement* );

        __checkReturn HRESULT InjectKey( 
            Key::Value Key, 
            KeyState::Value State, 
            __out_opt bool* pConsumed = NULL 
            );

        __checkReturn HRESULT InjectCharacter(
            WCHAR Character, 
            __out_opt bool* pConsumed = NULL 
            );

    protected:
        CKeyboardController(
            );

        virtual ~CKeyboardController(
            );

        __checkReturn HRESULT Initialize( 
            __in CFocusManager* pFocusManager,
            __in CUIElement* pRootElement 
            );

        CFocusManager* m_FocusManager;
        CUIElement* m_RootElement;
};
