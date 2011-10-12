#include "CGImagingProvider.h"
#include "CGBitmapSource.h"

#include <CoreGraphics/CGDataProvider.h>
#include <CoreGraphics/CGImage.h>

#include <iconv.h>

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
CCGImagingProvider::LoadBitmapFromStream(
    __in IReadStream* pStream,
    __deref_out CBitmapSource** ppBitmapSource
    )
{
    HRESULT hr = S_OK;
    CGDataProviderRef pDataProvider = NULL;
    CGImageRef pImage = NULL;
    CCGBitmapSource* pCGBitmapSource = NULL;
    CGDataProviderSequentialCallbacks providerCallbacks = { };
    
    providerCallbacks.version = 0;
    providerCallbacks.getBytes = &CCGImagingProvider::ProviderGetBytes;
    providerCallbacks.skipForward = &CCGImagingProvider::ProviderSkipForward;
    providerCallbacks.rewind = &CCGImagingProvider::ProviderRewind;
    providerCallbacks.releaseInfo = &CCGImagingProvider::ProviderReleaseInfo;
    
    AddRefObject(pStream);

    pDataProvider = CGDataProviderCreateSequential(pStream, &providerCallbacks);
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
    else
    {
        //
        // Release extra stream reference if a data provider wasn't created.
        //
        ReleaseObject(pStream);
    }
    
    if (pImage != NULL)
    {
        CGImageRelease(pImage);
    }
    
    return hr;
}

size_t 
CCGImagingProvider::ProviderGetBytes(
    void* pContext,
    void* pBuffer,
    size_t count
    )
{
    HRESULT hr = S_OK;
    IReadStream* pStream = (IReadStream*)pContext;
    UINT64 bytesWritten = 0;

    IFC(pStream->Read(pBuffer, count, &bytesWritten));

Cleanup:
    if (FAILED(hr))
    {
        bytesWritten = 0;
    }

    return bytesWritten;
}
                       
off_t 
CCGImagingProvider::ProviderSkipForward(
    void* pContext,
    off_t count
    )
{
    HRESULT hr = S_OK;
    IReadStream* pStream = (IReadStream*)pContext;
    UINT64 currentPosition = 0;
    UINT64 newPosition = 0;

    IFC(pStream->Seek(SeekType::Current, 0, &currentPosition));

    IFC(pStream->Seek(SeekType::Current, count, &newPosition));

Cleanup:
    if (FAILED(hr))
    {
        currentPosition = 0;
        newPosition = 0;
    }
    
    return newPosition - currentPosition;;
}
    
void
CCGImagingProvider::ProviderRewind(
    void* pContext
    )
{
    IReadStream* pStream = (IReadStream*)pContext;

    IGNOREHR(pStream->Seek(SeekType::Begin, 0, NULL));
}

void 
CCGImagingProvider::ProviderReleaseInfo(
    void* pContext
    )
{
    IReadStream* pStream = (IReadStream*)pContext;

    ReleaseObject(pStream);
}