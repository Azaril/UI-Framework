#pragma once

#include "RefCounted.h"
#include "UIElement.h"
#include "MouseInput.h"

class UIFRAMEWORK_API CFocusManager : public CRefCountedObject
{
    public:
        DECLARE_FACTORY( CFocusManager );

        __checkReturn HRESULT SetFocus( 
            __in CUIElement* pElement, 
            __out bool* pSetFocus 
            );

        __checkReturn HRESULT GetFocusedElement(
            __deref_out_opt CUIElement** ppElement 
            );

    protected:
        CFocusManager(
            );

        virtual ~CFocusManager(
            );

        __checkReturn HRESULT Initialize(
            );

        CUIElement* m_FocusedElement;
};
