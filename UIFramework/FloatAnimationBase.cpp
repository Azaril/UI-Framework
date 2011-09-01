#include "FloatAnimationBase.h"

CFloatAnimationBase::CFloatAnimationBase(
    )
{
}


CFloatAnimationBase::~CFloatAnimationBase(
    )
{
}

__override TypeIndex::Value 
CFloatAnimationBase::GetProvidedType(
    )
{
    return TypeIndex::Float;
}