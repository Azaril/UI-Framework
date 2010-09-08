#pragma once

#include "RefCounted.h"
#include "UIElement.h"
#include "MouseInput.h"
#include "FocusManager.h"

class UIFRAMEWORK_API CKeyboardController :  public CRefCountedObject 
{
    public:
        DECLARE_FACTORY2( CKeyboardController, CFocusManager*, CUIElement* );

        HRESULT InjectKey( UINT32 Key, KeyState::Value State, BOOL* pConsumed = NULL );
        HRESULT InjectCharacter( WCHAR Character, BOOL* pConsumed = NULL );

    protected:
        CKeyboardController();
        virtual ~CKeyboardController();

        HRESULT Initialize( CFocusManager* pFocusManager, CUIElement* pRootElement );

        CFocusManager* m_FocusManager;
        CUIElement* m_RootElement;
};
