#include "D2DLayer.h"

CD2DLayer::CD2DLayer(
	) 
	: m_Layer(NULL)
{
}

CD2DLayer::~CD2DLayer(
	)
{
    ReleaseObject(m_Layer);
}

__checkReturn HRESULT 
CD2DLayer::Initialize(
	ID2D1Layer* pLayer
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pLayer);

    m_Layer = pLayer;
    AddRefObject(m_Layer);
    
Cleanup:
    return hr;
}

__out ID2D1Layer* 
CD2DLayer::GetLayer(
	)
{
    return m_Layer;
}