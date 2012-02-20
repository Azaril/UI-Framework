#include "BitmapBrushBase.h"

CBitmapBrushBase::CBitmapBrushBase(
    )
    : m_pTexture(NULL)
{
}

CBitmapBrushBase::~CBitmapBrushBase(
    )
{
    ReleaseObject(m_pTexture);
}

__checkReturn HRESULT
CBitmapBrushBase::Initialize(
	__in ITexture* pTexture
	)
{
	HRESULT hr = S_OK;

	SetObject(m_pTexture, pTexture);
    
	return hr;
}

__override __out_opt ITexture*
CBitmapBrushBase::GetTexture(
	) const
{
	return m_pTexture;
}

__override ColorF 
CBitmapBrushBase::GetDiffuseColor(
    ) const
{
    return ColorF(1.0f, 1.0f, 1.0f, 1.0f);
}