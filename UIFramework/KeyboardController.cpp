#include "KeyboardController.h"
#include "KeyboardEventArgs.h"
#include "TextEventArgs.h"

CKeyboardController::CKeyboardController() : m_FocusManager(NULL),
                                             m_RootElement(NULL)
{
}

CKeyboardController::~CKeyboardController()
{
    ReleaseObject(m_FocusManager);
    ReleaseObject(m_RootElement);
}

HRESULT CKeyboardController::Initialize(CFocusManager* pFocusManager, CUIElement* pRootElement)
{
    HRESULT hr = S_OK;

    IFCPTR(pFocusManager);

    m_FocusManager = pFocusManager;
    AddRefObject(m_FocusManager);

    m_RootElement = pRootElement;
    AddRefObject(m_RootElement);

Cleanup:
    return hr;
}

HRESULT CKeyboardController::InjectKey(Key::Value Key, KeyState::Value State, BOOL* pConsumed)
{
    HRESULT hr = S_OK;
    CUIElement* pFocusedElement = NULL;
    CKeyEventArgs* pKeyEvent = NULL;

    IFC(m_FocusManager->GetFocusedElement(&pFocusedElement));

    if(pFocusedElement)
    {
        IFC(CKeyEventArgs::Create(&CUIElement::KeyEvent, Key, State, &pKeyEvent));

        IFC(pFocusedElement->RaiseEvent(pKeyEvent));
    }

    if(pConsumed)
    {
        *pConsumed = (pKeyEvent != NULL) ? pKeyEvent->IsHandled() : FALSE;
    }

Cleanup:
    ReleaseObject(pFocusedElement);
    ReleaseObject(pKeyEvent);

    return hr;
}

HRESULT CKeyboardController::InjectCharacter(WCHAR Character, BOOL* pConsumed)
{
    HRESULT hr = S_OK;
    CUIElement* pFocusedElement = NULL;
    CTextEventArgs* pTextEvent = NULL;

    IFC(m_FocusManager->GetFocusedElement(&pFocusedElement));

    if(pFocusedElement)
    {
        IFC(CTextEventArgs::Create(&CUIElement::TextEvent, &Character, 1, &pTextEvent));

        IFC(pFocusedElement->RaiseEvent(pTextEvent));
    }

    if(pConsumed)
    {
        *pConsumed = (pTextEvent != NULL) ? pTextEvent->IsHandled() : FALSE;
    }

Cleanup:
    ReleaseObject(pFocusedElement);
    ReleaseObject(pTextEvent);

    return hr;
}