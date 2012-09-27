#include "D2DBitmap.h"

#if defined(FRAMEWORK_D2D)

CD2DBitmap::CD2DBitmap(
	) 
	: m_Bitmap(NULL)
{
}

CD2DBitmap::~CD2DBitmap(
	)
{
    ReleaseObject(m_Bitmap);
}

__checkReturn HRESULT 
CD2DBitmap::Initialize(
	__in ID2D1Bitmap* pBitmap,
    const SizeU& size
	)
{
    HRESULT hr = S_OK;

    SetObject(m_Bitmap, pBitmap);

    m_Size = size;

    return hr;
}

__out ID2D1Bitmap* 
CD2DBitmap::GetD2DBitmap(
	)
{
    return m_Bitmap;
}

const SizeU&
CD2DBitmap::GetSize( 
    )
{
    return m_Size;
}

#endif