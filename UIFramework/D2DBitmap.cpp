#include "D2DBitmap.h"

CD2DBitmap::CD2DBitmap() : m_Bitmap(NULL)
{
}

CD2DBitmap::~CD2DBitmap()
{
    ReleaseObject(m_Bitmap);
}

HRESULT CD2DBitmap::Initialize(ID2D1Bitmap* pBitmap)
{
    HRESULT hr = S_OK;

    IFCPTR(pBitmap);

    m_Bitmap = pBitmap;
    AddRefObject(m_Bitmap);

Cleanup:
    return hr;
}

ID2D1Bitmap* CD2DBitmap::GetD2DBitmap()
{
    return m_Bitmap;
}