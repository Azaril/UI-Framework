#pragma once

#include "RefCounted.h"
#include "UIElement.h"
#include "MouseInput.h"

class CMouseController : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CMouseController, CUIElement* );

        HRESULT InjectMouseButton( MouseButton::Value Button, MouseButtonState::Value State, Point2F Location );
        HRESULT InjectMouseMove( Point2F Location );

        HRESULT SetMouseOverElement( CUIElement* pElement, Point2F Location );
        HRESULT RaiseMouseMove( Point2F Location );
        HRESULT RaiseMouseButton( Point2F Location, MouseButton::Value Button, MouseButtonState::Value State );

    protected:
        CMouseController();
        virtual ~CMouseController();

        HRESULT Initialize( CUIElement* pRootElement );

        CUIElement* m_RootElement;
        CUIElement* m_MouseOverElement;
        CUIElement* m_CaptureElement;
};