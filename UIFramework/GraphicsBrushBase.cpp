#include "GraphicsBrushBase.h"

CGraphicsBrushBase::CGraphicsBrushBase(
    )
    : m_Transform(Matrix3X2F::Identity())
{
}

CGraphicsBrushBase::~CGraphicsBrushBase(
    )
{
}

__override __checkReturn HRESULT
CGraphicsBrushBase::SetTransform(
    const Matrix3X2F& Transform
    )
{
	HRESULT hr = S_OK;
    
	m_Transform = Transform;
    
	return hr;
}

__override void
CGraphicsBrushBase::GetTransform(
    Matrix3X2F& Transform
    ) const
{
    Transform = m_Transform;
}

void
CGraphicsBrushBase::GetFinalTransform(
    Matrix3X2F& Transform
    ) const
{
    GetTransform(Transform);
}