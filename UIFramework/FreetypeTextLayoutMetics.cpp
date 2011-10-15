#include "FreetypeTextLayoutMetics.h"

CFreetypeTextLayoutMetics::CFreetypeTextLayoutMetics(
    )
{
}

CFreetypeTextLayoutMetics::~CFreetypeTextLayoutMetics(
    )
{
}

__checkReturn HRESULT
CFreetypeTextLayoutMetics::Initialize(
    const RectF& bounds
    )
{
    HRESULT hr = S_OK;

    m_Bounds = bounds;

    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextLayoutMetics::GetBounds(
    __out RectF* pBounds
    )
{
    HRESULT hr = S_OK;

    *pBounds = m_Bounds;

    return hr;
}