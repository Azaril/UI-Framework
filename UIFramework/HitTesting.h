#pragma once

#include "Visual.h"

namespace HitTestResultBehavior
{
    enum Value
    {
        Continue,
        Stop
    };
}

namespace HitTestFilterBehavior
{
    enum Value
    {
        Continue,
        ContinueSkipChildren,
        ContinueSkipSelf,
        ContinueSkipSelfAndChildren,
        Stop
    };
}

class CHitTestFilter
{
    public:
        virtual HRESULT Filter( CVisual* pVisual, HitTestFilterBehavior::Value* pFilterBehavior ) = 0;
};

class CHitTestCallback
{
    public:
        virtual HRESULT ItemHit( CVisual* pVisual, HitTestResultBehavior::Value* pResultBehavior ) = 0;
};

HRESULT HitTestTree( CVisual* pRootVisual, const Point2F& LocalPoint, CHitTestFilter* pFilter, CHitTestCallback* pCallback );
