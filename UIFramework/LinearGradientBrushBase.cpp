#include "LinearGradientBrushBase.h"

CLinearGradientBrushBase::CLinearGradientBrushBase(
	)
	: m_pTexture(NULL)
{
}

CLinearGradientBrushBase::~CLinearGradientBrushBase(
	)
{
	ReleaseObject(m_pTexture);
}

__checkReturn HRESULT
CLinearGradientBrushBase::Initialize(
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
CLinearGradientBrushBase::GetTexture(
	) const
{
	return m_pTexture;
}

__override ColorF 
CLinearGradientBrushBase::GetDiffuseColor(
    ) const
{
    return ColorF(1.0f, 1.0f, 1.0f, 1.0f);
}

__override void
CLinearGradientBrushBase::GetFinalTransform(
    Matrix3X2F& Transform
    ) const
{
    Matrix3X2F baseTransform;
    
    GetTransform(baseTransform);
    
    Transform = m_TextureToBrushTransform * baseTransform;
}