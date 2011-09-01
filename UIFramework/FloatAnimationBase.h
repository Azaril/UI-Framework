#pragma once

#include "AnimationTimeline.h"

class UIFRAMEWORK_API CFloatAnimationBase : public CAnimationTimeline
{
    public:
        __override virtual TypeIndex::Value GetProvidedType(
            );

    protected:
        CFloatAnimationBase(
            );

        virtual ~CFloatAnimationBase(
            );
};

