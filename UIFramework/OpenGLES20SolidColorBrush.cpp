#include "OpenGLES20SolidColorBrush.h"

COpenGLES20SolidColorBrush::COpenGLES20SolidColorBrush(
	)
{
}

COpenGLES20SolidColorBrush::~COpenGLES20SolidColorBrush(
	)
{
}

__checkReturn HRESULT
COpenGLES20SolidColorBrush::Initialize(
	const ColorF& Color
	)
{
	HRESULT hr = S_OK;

	m_Color = Color;

	return hr;
}

__checkReturn HRESULT
COpenGLES20SolidColorBrush::SetTransform(
	const Matrix3X2F& Transform
	)
{
	HRESULT hr = S_OK;

	//
	// Transform has no effect on solid color brushes.
	//

	return hr;
}

__override const ColorF&
COpenGLES20SolidColorBrush::GetDiffuseColor(
    )
{
    return m_Color;
}