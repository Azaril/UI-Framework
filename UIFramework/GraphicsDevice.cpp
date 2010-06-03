#include "GraphicsDevice.h"

#ifdef __WIN32__
#include "D2DGraphicsDevice.h"
#endif

HRESULT CreateGraphicsDevice(CGraphicsDevice** ppGraphicsDevice)
{
    HRESULT hr = S_OK;
    
#ifdef __WIN32__    
    CD2DGraphicsDevice* pD2DDevice = NULL;
#endif

    IFCPTR(ppGraphicsDevice);

#ifdef __WIN32__
    if(SUCCEEDED(CD2DGraphicsDevice::Create(&pD2DDevice)))
    {
        *ppGraphicsDevice = pD2DDevice;
        pD2DDevice = NULL;
        goto Cleanup;
    }
#endif
    
    IFC(E_FAIL);

Cleanup:
    
#ifdef __WIN32__
    ReleaseObject(pD2DDevice);
#endif

    return hr;
}