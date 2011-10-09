#include "SolidColorBrushBase.h"

CSolidColorBrushBase::CSolidColorBrushBase(
	)
{
}

CSolidColorBrushBase::~CSolidColorBrushBase(
	)
{
}

__checkReturn HRESULT
CSolidColorBrushBase::Initialize(
	const ColorF& Color
	)
{
	HRESULT hr = S_OK;

	m_Color = Color;

	return hr;
}

__override ColorF
CSolidColorBrushBase::GetDiffuseColor(
    ) const
{
    return m_Color;
}

__override __out_opt ITexture*
CSolidColorBrushBase::GetTexture(
    ) const
{
	return NULL;
}