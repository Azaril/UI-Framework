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
        virtual __checkReturn HRESULT Filter( 
            __in CVisual* pVisual, 
            __out HitTestFilterBehavior::Value* pFilterBehavior 
            ) = 0;
};

class CHitTestCallback
{
    public:
        virtual __checkReturn HRESULT ItemHit( 
            __in CVisual* pVisual, 
            __out HitTestResultBehavior::Value* pResultBehavior 
            ) = 0;
};

__checkReturn HRESULT HitTestTree( 
    __in CVisual* pRootVisual, 
    const Point2F& LocalPoint, 
    __in_opt CHitTestFilter* pFilter, 
    __in CHitTestCallback* pCallback 
    );
