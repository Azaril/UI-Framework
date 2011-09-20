#include "OpenGLES20Brush.h"

COpenGLES20Brush::COpenGLES20Brush(
    )
    : m_Transform(Matrix3X2F::Identity())
{
}

COpenGLES20Brush::~COpenGLES20Brush(
    )
{
}

__checkReturn HRESULT
COpenGLES20Brush::SetTransform(
    const Matrix3X2F& Transform
    )
{
	HRESULT hr = S_OK;
    
	m_Transform = Transform;
    
	return hr;
}

void
COpenGLES20Brush::GetTransform(
    Matrix3X2F& Transform
    ) const
{
    Transform = m_Transform;
}