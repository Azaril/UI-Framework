#include "ScrollViewer.h"

CScrollViewer::CScrollViewer()
{
}

CScrollViewer::~CScrollViewer()
{
}

HRESULT CScrollViewer::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CContentControl::Initialize(pProviders));

Cleanup:
    return hr;
}

HRESULT CScrollViewer::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}