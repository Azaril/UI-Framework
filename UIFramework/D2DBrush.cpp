#include "D2DBrush.h"
#include "ErrorChecking.h"

CD2DBrush::CD2DBrush() : m_Brush(NULL)
{
}

CD2DBrush::~CD2DBrush()
{
    ReleaseObject(m_Brush);
}

HRESULT CD2DBrush::Initialize(ID2D1Brush* pBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(pBrush);

    m_Brush = pBrush;
    AddRefObject(m_Brush);

Cleanup:
    return hr;
}

ID2D1Brush* CD2DBrush::GetD2DBrush()
{
    return m_Brush;
}