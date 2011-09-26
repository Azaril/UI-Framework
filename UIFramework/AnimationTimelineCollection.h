#pragma once

#include "Collection.h"
#include "AnimationTimeline.h"

class CAnimationTimelineCollection : public CCollection< CAnimationTimeline >
{
    public:
        DECLARE_FACTORY( CAnimationTimelineCollection );

        DECLARE_TYPE_WITH_BASE( TypeIndex::AnimationTimelineCollection, CCollection< CAnimationTimeline > );
};

template< >
struct ObjectTypeTraits< CAnimationTimelineCollection >
{
    static const TypeIndex::Value Type = TypeIndex::AnimationTimelineCollection;
};