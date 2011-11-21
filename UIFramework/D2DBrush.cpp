#include "D2DBrush.h"
#include "ErrorChecking.h"
#include "D2DUtilities.h"

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

__override __checkReturn HRESULT 
CD2DBrush::GetBounds( 
    RectF& brushBounds
    )
{
    HRESULT hr = S_OK;

    brushBounds = RectF(0.0, 0.0, 1.0, 1.0);

    return hr;
}

__override __checkReturn HRESULT 
CD2DBrush::SetTransform(
	const Matrix3X2F& Transform
	)
{
    HRESULT hr = S_OK;

    m_Brush->SetTransform(Matrix3X2FToD2DMatrix3X2F(&Transform));

    return hr;
}

__override void
CD2DBrush::GetTransform(
    Matrix3X2F& Transform
    ) const
{
    return m_Brush->GetTransform(Matrix3X2FToD2DMatrix3X2F(&Transform));
}