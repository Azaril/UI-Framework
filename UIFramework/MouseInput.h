#pragma once

#include "HitTesting.h"
#include "Enums.h"
#include "MouseInputEventArgs.h"

class CMouseController;

class CMouseInputHitTestFilter : public CHitTestFilter
{
    public:
        virtual HRESULT Filter( CVisual* pVisual, HitTestFilterBehavior::Value* pFilterBehavior );
};

class CMouseButtonHitTestCallback : public CHitTestCallback
{
    public:
        CMouseButtonHitTestCallback( CMouseController* pController, Point2F MouseLocation, MouseButton::Value Button, MouseButtonState::Value State );

        virtual HRESULT ItemHit( CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior );

    protected:
        CMouseController* m_Controller;
        MouseButton::Value m_Button;
        MouseButtonState::Value m_ButtonState;
        Point2F m_Location;
};

class CMouseMoveHitTestCallback : public CHitTestCallback
{
    public:
        CMouseMoveHitTestCallback( CMouseController* pController, Point2F MouseLocation );

        virtual HRESULT ItemHit( CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior );

    protected:
        CMouseController* m_Controller;
        Point2F m_Location;
};