#pragma once

#include <math.h>
#include "DataTypes.h"

#define FLOAT_EPSILON (0.001f)

inline bool IsFloatNear(
    const FLOAT& first,
    const FLOAT& second
    )
{
    return (fabs(first - second) < FLOAT_EPSILON);
}

inline bool IsFloatLessThan(
    const FLOAT& first,
    const FLOAT& second
    )
{
    return (!IsFloatNear(first, second) && (first < second));
}

inline bool IsFloatGreaterThan(
    const FLOAT& first,
    const FLOAT& second
    )
{
    return (!IsFloatNear(first, second) && (first > second));
}

inline bool IsFloatLessOrEqualThan(
    const FLOAT& first,
    const FLOAT& second
    )
{
    return (IsFloatNear(first, second) || (first <= second));
}

inline bool IsFloatGreaterOrEqualThan(
    const FLOAT& first,
    const FLOAT& second
    )
{
    return (IsFloatNear(first, second) || (first >= second));
}