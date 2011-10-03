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

__override ColorF
COpenGLES20SolidColorBrush::GetDiffuseColor(
    ) const
{
    return m_Color;
}

__override __out_opt ITexture*
COpenGLES20SolidColorBrush::GetTexture(
    ) const
{
	return NULL;
}