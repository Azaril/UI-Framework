#pragma once

#include "HitTesting.h"
#include "Enums.h"
#include "MouseInputEventArgs.h"

class CMouseController;

class CMouseInputHitTestFilter : public CHitTestFilter
{
    public:
        __override virtual __checkReturn HRESULT Filter(
            __in CVisual* pVisual,
            __out HitTestFilterBehavior::Value* pFilterBehavior 
            );
};

class CMouseButtonHitTestCallback : public CHitTestCallback
{
    public:
        CMouseButtonHitTestCallback( 
            __in CMouseController* pController,
            Point2F MouseLocation,
            MouseButton::Value Button, 
            MouseButtonState::Value State, 
            __out_opt BOOL* pHandled 
            );

        __override virtual __checkReturn HRESULT ItemHit( 
            __in CVisual* pVisual, 
            __out HitTestResultBehavior::Value* pResultBehavior 
            );

    protected:
        CMouseController* m_Controller;
        MouseButton::Value m_Button;
        MouseButtonState::Value m_ButtonState;
        Point2F m_Location;
        BOOL* m_Handled;
};

class CMouseMoveHitTestCallback : public CHitTestCallback
{
    public:
        CMouseMoveHitTestCallback( 
            __in CMouseController* pController,
            Point2F MouseLocation,
            __out_opt BOOL* pHandled 
            );

        __override virtual __checkReturn HRESULT ItemHit( 
            __in CVisual* pVisual,
            __out HitTestResultBehavior::Value* pResultBehavior 
            );

    protected:
        CMouseController* m_Controller;
        Point2F m_Location;
        BOOL* m_Handled;
};