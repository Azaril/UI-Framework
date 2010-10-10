#pragma once

#include "AnimationTimeline.h"

class UIFRAMEWORK_API CFloatAnimationBase : public CAnimationTimeline
{
    public:
        virtual TypeIndex::Value GetProvidedType();

    protected:
        CFloatAnimationBase();
        virtual ~CFloatAnimationBase();
};

