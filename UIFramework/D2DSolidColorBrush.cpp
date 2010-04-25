#include "D2DSolidColorBrush.h"

CD2DSolidColorBrush::CD2DSolidColorBrush() : m_SolidBrush(NULL)
{
}

CD2DSolidColorBrush::~CD2DSolidColorBrush()
{
    ReleaseObject(m_SolidBrush);
}

HRESULT CD2DSolidColorBrush::Initialize(ID2D1SolidColorBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(pBrush);

    IFC(CD2DBrush::Initialize(pBrush));

    m_SolidBrush = pBrush;
    AddRefObject(m_SolidBrush);

Cleanup:
    return hr;
}
