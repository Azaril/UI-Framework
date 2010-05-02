#pragma once

#include "HitTesting.h"

class CMouseInputHitTestFilter : public CHitTestFilter
{
    public:
        virtual HRESULT Filter(CVisual* pVisual, HitTestFilterBehavior::Value* pFilterBehavior);
};

class CMouseInputHitTestCallback : public CHitTestCallback
{
    public:
        virtual HRESULT ItemHit(CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior);
};