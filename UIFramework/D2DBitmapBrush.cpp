#include "D2DBitmapBrush.h"

CD2DBitmapBrush::CD2DBitmapBrush(
	) 
	: m_BitmapBrush(NULL)
{
}

CD2DBitmapBrush::~CD2DBitmapBrush(
	)
{
    ReleaseObject(m_BitmapBrush);
}

__checkReturn HRESULT 
CD2DBitmapBrush::Initialize(
	__in ID2D1BitmapBrush* pBrush,
    const SizeU& size
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pBrush);

    IFC(CD2DBrush::Initialize(pBrush));

    m_BitmapBrush = pBrush;
    AddRefObject(m_BitmapBrush);

    m_Bounds = RectF(0, 0, (FLOAT)size.width, (FLOAT)size.height);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CD2DBitmapBrush::GetBounds( 
    RectF& brushBounds
    )
{
    HRESULT hr = S_OK;

    brushBounds = m_Bounds;

    return hr;
}