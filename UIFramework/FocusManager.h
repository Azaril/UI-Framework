#pragma once

#include "RefCounted.h"
#include "UIElement.h"
#include "MouseInput.h"

class UIFRAMEWORK_API CFocusManager : public CRefCountedObject
{
    public:
        DECLARE_FACTORY( CFocusManager );

        HRESULT SetFocus( CUIElement* pElement, BOOL* pSetFocus );
        HRESULT GetFocusedElement( CUIElement** ppElement );

    protected:
        CFocusManager();
        virtual ~CFocusManager();

        HRESULT Initialize();

        CUIElement* m_FocusedElement;
};
