#include "GraphicsDevice.h"

#ifdef _WINDOWS
#include "D2DGraphicsDevice.h"
#endif

HRESULT CreateGraphicsDevice(CGraphicsDevice** ppGraphicsDevice)
{
    HRESULT hr = S_OK;
    
#ifdef _WINDOWS    
    CD2DGraphicsDevice* pD2DDevice = NULL;
#endif

    IFCPTR(ppGraphicsDevice);

#ifdef _WINDOWS
    if(SUCCEEDED(CD2DGraphicsDevice::Create(&pD2DDevice)))
    {
        *ppGraphicsDevice = pD2DDevice;
        pD2DDevice = NULL;
        goto Cleanup;
    }
#endif
    
    IFC(E_FAIL);

Cleanup:
    
#ifdef _WINDOWS
    ReleaseObject(pD2DDevice);
#endif

    return hr;
}