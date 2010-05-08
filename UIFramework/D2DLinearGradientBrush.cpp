#include "D2DLinearGradientBrush.h"

CD2DLinearGradientBrush::CD2DLinearGradientBrush() : m_LinearGradientBrush(NULL)
{
}

CD2DLinearGradientBrush::~CD2DLinearGradientBrush()
{
    ReleaseObject(m_LinearGradientBrush);
}

HRESULT CD2DLinearGradientBrush::Initialize(ID2D1LinearGradientBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(pBrush);

    IFC(CD2DBrush::Initialize(pBrush));

    m_LinearGradientBrush = pBrush;
    AddRefObject(m_LinearGradientBrush);

Cleanup:
    return hr;
}