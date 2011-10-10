#include "CGImagingProvider.h"
#include "CGBitmapSource.h"

#include <CoreGraphics/CGDataProvider.h>
#include <CoreGraphics/CGImage.h>

CCGImagingProvider::CCGImagingProvider(
    )
{
}

CCGImagingProvider::~CCGImagingProvider(
    )
{
}

__checkReturn HRESULT
CCGImagingProvider::Initialize(
    )
{
    HRESULT hr = S_OK;
    
    return hr;
}

__override __checkReturn HRESULT 
CCGImagingProvider::LoadBitmapFromFile(
  __in_z const WCHAR* pPath,
  __deref_out CBitmapSource** ppBitmapSource
  )
{
    HRESULT hr = S_OK;
    CGDataProviderRef pDataProvider = NULL;
    CGImageRef pImage = NULL;
    CCGBitmapSource* pCGBitmapSource = NULL;
    
    //TODO: Convert path and resolve.
    pDataProvider = CGDataProviderCreateWithFilename("foo");
    IFCPTR(pDataProvider);
    
    if (pImage == NULL)
    {
        pImage = CGImageCreateWithPNGDataProvider(pDataProvider, NULL, false, kCGRenderingIntentDefault);
    }
    
    if (pImage == NULL)
    {
        pImage = CGImageCreateWithJPEGDataProvider(pDataProvider, NULL, false, kCGRenderingIntentDefault);
    }
    
    IFCPTR(pImage);
    
    IFC(CCGBitmapSource::Create(pImage, &pCGBitmapSource));
    
    *ppBitmapSource = pCGBitmapSource;
    pCGBitmapSource = NULL;

Cleanup:
    ReleaseObject(pCGBitmapSource);
    
    if (pDataProvider != NULL)
    {
        CGDataProviderRelease(pDataProvider);
    }
    
    if (pImage != NULL)
    {
        CGImageRelease(pImage);
    }
    
    return hr;
}

__override __checkReturn HRESULT 
CCGImagingProvider::LoadBitmapFromMemory(
    __in_bcount(DataSize) const BYTE* pData,
    UINT32 DataSize,
    __deref_out CBitmapSource** ppBitmapSource
    )
{
    HRESULT hr = S_OK;

    //TODO: Implement.

    return hr;
}