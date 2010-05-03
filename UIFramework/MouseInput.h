#pragma once

#include "HitTesting.h"
#include "Enums.h"
#include "MouseInputEventArgs.h"

class CMouseInputHitTestFilter : public CHitTestFilter
{
    public:
        virtual HRESULT Filter( CVisual* pVisual, HitTestFilterBehavior::Value* pFilterBehavior );
};

class CMouseButtonHitTestCallback : public CHitTestCallback
{
    public:
        CMouseButtonHitTestCallback( Point2F MouseLocation, MouseButton::Value Button, MouseButtonState::Value State );

        virtual HRESULT ItemHit( CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior );

    protected:
        MouseButton::Value m_Button;
        MouseButtonState::Value m_ButtonState;
        Point2F m_Location;
};

class CMouseMoveHitTestCallback : public CHitTestCallback
{
    public:
        CMouseMoveHitTestCallback( Point2F MouseLocation );

        virtual HRESULT ItemHit( CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior );

    protected:
        Point2F m_Location;
};