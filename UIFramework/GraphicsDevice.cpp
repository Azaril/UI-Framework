#include "GraphicsDevice.h"
#include "D2DGraphicsDevice.h"

HRESULT CreateGraphicsDevice(CGraphicsDevice** ppGraphicsDevice)
{
    HRESULT hr = S_OK;
    CD2DGraphicsDevice* pD2DDevice = NULL;

    IFCPTR(ppGraphicsDevice);

    if(SUCCEEDED(CD2DGraphicsDevice::Create(&pD2DDevice)))
    {
        *ppGraphicsDevice = pD2DDevice;
        pD2DDevice = NULL;
        goto Cleanup;
    }

Cleanup:
    ReleaseObject(pD2DDevice);

    return hr;
}