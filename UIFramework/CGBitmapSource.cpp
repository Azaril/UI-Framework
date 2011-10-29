#include "CGBitmapSource.h"

#include <CoreGraphics/CGContext.h>
#include <CoreGraphics/CGBitmapContext.h>

CCGBitmapSource::CCGBitmapSource(
    )
    : m_pImage(NULL)
{
}

CCGBitmapSource::~CCGBitmapSource(
	)
{
	if (m_pImage != NULL)
	{
		CGImageRelease(m_pImage);
	}
}

__checkReturn HRESULT
CCGBitmapSource::Initialize(
	__in CGImageRef pImage
	)
{
	HRESULT hr = S_OK;

	m_pImage = pImage;
	CGImageRetain(m_pImage);

	return hr;
}

__checkReturn HRESULT
CCGBitmapSource::GetSize(
	__out SizeU* pSize
	)
{
	HRESULT hr = S_OK;

	pSize->width = CGImageGetWidth(m_pImage);
	pSize->height = CGImageGetHeight(m_pImage);

	return hr;
}

__checkReturn HRESULT
CCGBitmapSource::LoadIntoTexture(
	__in ITexture* pTexture
	)
{
	HRESULT hr = S_OK;
	CGColorSpaceRef pColorSpace = NULL;
	CGContextRef pContext = NULL;
	BYTE* pDecodedImage = NULL;
	UINT32 lineSize = 0;
	UINT32 bufferSize = 0;
	PixelFormat::Value pixelFormat = PixelFormat::Unknown;
	SizeU sourceSize;

	pixelFormat = pTexture->GetPixelFormat();

	IFC(GetSize(&sourceSize));

	switch (pixelFormat)
	{
		case PixelFormat::B8G8R8A8:
        case PixelFormat::R8G8B8A8:
			{
				pColorSpace = CGColorSpaceCreateDeviceRGB();

				break;
			}

		default:
			{
				IFC(E_UNEXPECTED);
			}
	}

	IFCPTR(pColorSpace);

	lineSize = PixelFormat::GetLineSize(pixelFormat, sourceSize.width);
	bufferSize = lineSize * sourceSize.height;
	pDecodedImage = new BYTE[bufferSize];

	switch (pixelFormat)
	{
		case PixelFormat::B8G8R8A8:
			{
				pContext = CGBitmapContextCreate(pDecodedImage, sourceSize.width, sourceSize.height, 8, lineSize, pColorSpace, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little);

				break;
			}
            
		case PixelFormat::R8G8B8A8:
            {
                pContext = CGBitmapContextCreate(pDecodedImage, sourceSize.width, sourceSize.height, 8, lineSize, pColorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
                
                break;
            }            

		default:
			{
				IFC(E_UNEXPECTED);
			}
	}

	IFCPTR(pContext);

    CGContextTranslateCTM(pContext, 0, 0);
	CGContextClearRect(pContext, CGRectMake(0, 0, sourceSize.width, sourceSize.height));
	CGContextDrawImage(pContext, CGRectMake(0, 0, sourceSize.width, sourceSize.height), m_pImage);

	IFC(pTexture->SetSubData(MakeRect(sourceSize), pDecodedImage, bufferSize, lineSize));

Cleanup:
	if (pColorSpace != NULL)
	{
		CGColorSpaceRelease(pColorSpace);
	}

	if (pContext != NULL)
	{
		CGContextRelease(pContext);
	}

	delete [] pDecodedImage;

	return hr;
}