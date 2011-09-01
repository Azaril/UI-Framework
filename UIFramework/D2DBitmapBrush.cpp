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
	__in ID2D1BitmapBrush* pBrush
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pBrush);

    IFC(CD2DBrush::Initialize(pBrush));

    m_BitmapBrush = pBrush;
    AddRefObject(m_BitmapBrush);

Cleanup:
    return hr;
}