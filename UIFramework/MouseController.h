#pragma once

#include "RefCounted.h"
#include "UIElement.h"
#include "MouseInput.h"
#include "FocusManager.h"

class UIFRAMEWORK_API CMouseController : public CRefCountedObject
{
    public:
        DECLARE_FACTORY( CMouseController );

        HRESULT SetRootElement( CUIElement* pRootElement );

        HRESULT InjectMouseButton( MouseButton::Value Button, MouseButtonState::Value State, BOOL* pConsumed = NULL );
        HRESULT InjectMouseMove( Point2F Location, BOOL* pConsumed = NULL );

        HRESULT SetMouseOverElement( CUIElement* pElement, Point2F Location );
        HRESULT RaiseMouseMove( Point2F Location, BOOL* pConsumed = NULL );
        HRESULT RaiseMouseButton( Point2F Location, MouseButton::Value Button, MouseButtonState::Value State, BOOL* pConsumed = NULL );

        HRESULT SetCapture( CUIElement* pElement );
        HRESULT ReleaseCapture( CUIElement* pElement );

    protected:
        CMouseController();
        virtual ~CMouseController();

        HRESULT Initialize();

        CUIElement* m_RootElement;
        CUIElement* m_MouseOverElement;
        CUIElement* m_CaptureElement;

        Point2F m_Position;
};