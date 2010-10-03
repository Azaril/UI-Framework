#pragma once

#include "RefCounted.h"
#include "UIElement.h"
#include "MouseInput.h"
#include "FocusManager.h"

class UIFRAMEWORK_API CKeyboardController :  public CRefCountedObject 
{
    public:
        DECLARE_FACTORY1( CKeyboardController, CFocusManager* );

        HRESULT SetRootElement( CUIElement* pRootElement );

        HRESULT InjectKey( Key::Value Key, KeyState::Value State, BOOL* pConsumed = NULL );
        HRESULT InjectCharacter( WCHAR Character, BOOL* pConsumed = NULL );

    protected:
        CKeyboardController();
        virtual ~CKeyboardController();

        HRESULT Initialize( CFocusManager* pFocusManager );

        CFocusManager* m_FocusManager;
        CUIElement* m_RootElement;
};
