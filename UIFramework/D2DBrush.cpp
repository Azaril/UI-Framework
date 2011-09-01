#include "D2DBrush.h"
#include "ErrorChecking.h"

CD2DBrush::CD2DBrush(
	) 
	: m_Brush(NULL)
{
}

CD2DBrush::~CD2DBrush(
	)
{
    ReleaseObject(m_Brush);
}

HRESULT 
CD2DBrush::Initialize(
	__in ID2D1Brush* pBrush
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pBrush);

    m_Brush = pBrush;
    AddRefObject(m_Brush);

Cleanup:
    return hr;
}

__out ID2D1Brush* 
CD2DBrush::GetD2DBrush(
	)
{
    return m_Brush;
}

__checkReturn HRESULT 
CD2DBrush::SetTransform(
	const Matrix3X2F& Transform
	)
{
    HRESULT hr = S_OK;

    m_Brush->SetTransform(Transform);

    return hr;
}