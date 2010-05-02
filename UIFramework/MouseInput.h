#pragma once

#include "HitTesting.h"
#include "Enums.h"

class CMouseInputHitTestFilter : public CHitTestFilter
{
    public:
        virtual HRESULT Filter(CVisual* pVisual, HitTestFilterBehavior::Value* pFilterBehavior);
};

class CMouseDownHitTestCallback : public CHitTestCallback
{
    public:
        CMouseDownHitTestCallback( MouseButton::Value Button );

        virtual HRESULT ItemHit(CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior);

    protected:
        MouseButton::Value m_Button;
};