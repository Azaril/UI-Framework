#pragma once

#include "RefCounted.h"
#include "UIElement.h"
#include "MouseInput.h"
#include "FocusManager.h"

class UIFRAMEWORK_API CMouseController : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CMouseController, CUIElement* );

        __checkReturn HRESULT InjectMouseButton( 
            MouseButton::Value Button, 
            MouseButtonState::Value State, 
            __out_opt BOOL* pConsumed = NULL 
            );

        __checkReturn HRESULT InjectMouseMove( 
            Point2F Location,
            __out_opt BOOL* pConsumed = NULL 
            );

        __checkReturn HRESULT SetMouseOverElement( 
            __in_opt CUIElement* pElement, 
            Point2F Location
            );

        __checkReturn HRESULT RaiseMouseMove( 
            Point2F Location, 
            __out_opt BOOL* pConsumed = NULL 
            );

        __checkReturn HRESULT RaiseMouseButton( 
            Point2F Location,
            MouseButton::Value Button, 
            MouseButtonState::Value State, 
            __out_opt BOOL* pConsumed = NULL 
            );

        __checkReturn HRESULT SetCapture( 
            __in CUIElement* pElement 
            );

        __checkReturn HRESULT ReleaseCapture( 
            __in CUIElement* pElement 
            );

    protected:
        CMouseController(
            );

        virtual ~CMouseController(
            );

        __checkReturn HRESULT Initialize( 
            __in CUIElement* pRootElement 
            );

        CUIElement* m_RootElement;
        CUIElement* m_MouseOverElement;
        CUIElement* m_CaptureElement;

        Point2F m_Position;
};