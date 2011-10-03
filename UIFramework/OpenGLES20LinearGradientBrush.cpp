#include "OpenGLES20LinearGradientBrush.h"

COpenGLES20LinearGradientBrush::COpenGLES20LinearGradientBrush(
	)
	: m_pTexture(NULL)
{
}

COpenGLES20LinearGradientBrush::~COpenGLES20LinearGradientBrush(
	)
{
	ReleaseObject(m_pTexture);
}

__checkReturn HRESULT
COpenGLES20LinearGradientBrush::Initialize(
	__in ITexture* pTexture,
    const Matrix3X2F& textureToBrushTransform
	)
{
	HRESULT hr = S_OK;

	SetObject(m_pTexture, pTexture);
    
    m_TextureToBrushTransform = textureToBrushTransform;

	return hr;
}

__override __out_opt ITexture*
COpenGLES20LinearGradientBrush::GetTexture(
	) const
{
	return m_pTexture;
}

__override ColorF 
COpenGLES20LinearGradientBrush::GetDiffuseColor(
    ) const
{
    return ColorF(1.0f, 1.0f, 1.0f, 1.0f);
}

__override void
COpenGLES20LinearGradientBrush::GetFinalTransform(
    Matrix3X2F& Transform
    ) const
{
    Matrix3X2F baseTransform;
    
    GetTransform(baseTransform);
    
    Transform = m_TextureToBrushTransform * baseTransform;
}