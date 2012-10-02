#include "WICBitmapSource.h"

#if defined(FRAMEWORK_WIC)

CWICBitmapSource::CWICBitmapSource(
	) 
    : m_pSource(NULL)
    , m_pFactory(NULL)
	, m_pStream(NULL)
{
}

CWICBitmapSource::~CWICBitmapSource(
	)
{
    ReleaseObject(m_pSource);
    ReleaseObject(m_pStream);
    ReleaseObject(m_pFactory);
}

__checkReturn HRESULT 
CWICBitmapSource::Initialize(
    __in IWICImagingFactory* pFactory,
	__in IWICBitmapSource* pSource
	)
{
    HRESULT hr = S_OK;

    SetObject(m_pFactory, pFactory);
    SetObject(m_pSource, pSource);

    return hr;
}

__out IWICBitmapSource* 
CWICBitmapSource::GetWICBitmapSource(
	)
{
    return m_pSource;
}

__override __checkReturn HRESULT 
CWICBitmapSource::GetSize(
	__out SizeU* pSize
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pSize);

    IFC(m_pSource->GetSize(&pSize->width, &pSize->height));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CWICBitmapSource::AssociateStream(
	__in IStream* pStream
	)
{
    HRESULT hr = S_OK;

    ReplaceObject(m_pStream, pStream);

    return hr;
}

__checkReturn HRESULT
PixelFormatToBestWICFormat(
    PixelFormat::Value source,
    __out WICPixelFormatGUID* pTargetWICFormat,
    __out PixelFormat::Value* pTargetPixelFormat
    )
{
    HRESULT hr = S_OK;

    switch(source)
    {
        case PixelFormat::B8G8R8A8:
        case PixelFormat::A8R8G8B8:
            {
                *pTargetWICFormat = GUID_WICPixelFormat32bppBGRA;
                *pTargetPixelFormat = PixelFormat::B8G8R8A8;

                break;
            }

        case PixelFormat::R8G8B8A8:
		case PixelFormat::A8B8G8R8:
            {
                *pTargetWICFormat = GUID_WICPixelFormat32bppRGBA;
                *pTargetPixelFormat = PixelFormat::R8G8B8A8;

                break;
            }

        default:
            {
                IFC(E_FAIL);
            }
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CWICBitmapSource::LoadIntoTexture( 
    __in ITexture* pTexture 
    )
{
    HRESULT hr = S_OK;
    WICPixelFormatGUID targetFormat;
    BYTE* pDecodedPixels = NULL;
    IWICBitmapSource* pDecodedSource = NULL;
    PixelFormat::Value targetPixelFormat = PixelFormat::Unknown;

    IFC(PixelFormatToBestWICFormat(pTexture->GetPixelFormat(), &targetFormat, &targetPixelFormat));

    IFC(GetSourceAsFormat(targetFormat, &pDecodedSource));

    {
        SizeU sourceSize;

        IFC(GetSize(&sourceSize));

        IFCEXPECT(sourceSize.width <= pTexture->GetWidth());
        IFCEXPECT(sourceSize.height <= pTexture->GetHeight());

        UINT32 lineSize = PixelFormat::GetLineSize(targetPixelFormat, sourceSize.width);
        UINT32 bufferSize = lineSize * pTexture->GetHeight();

        pDecodedPixels = new BYTE[bufferSize];
        IFCOOM(pDecodedPixels);

        IFC(pDecodedSource->CopyPixels(NULL, lineSize, bufferSize, pDecodedPixels));

        if (pTexture->GetPixelFormat() == targetPixelFormat)
        {
            IFC(pTexture->SetSubData(MakeRect(sourceSize), pDecodedPixels, bufferSize, lineSize));
        }
        else
        {
            //TODO: wiarchbe: Change bytes to match endianness etc.
            ASSERT(false);
        }
    }    

Cleanup:
    ReleaseObject(pDecodedSource);
    
    delete [] pDecodedPixels;

    return hr;
}

__checkReturn HRESULT
CWICBitmapSource::GetSourceAsFormat(
    const WICPixelFormatGUID& format,
    IWICBitmapSource** ppSource
    )
{
    HRESULT hr = S_OK;
    WICPixelFormatGUID sourceFormat;
    IWICFormatConverter* pConverter = NULL;

    IFC(m_pSource->GetPixelFormat(&sourceFormat));

    if (format != sourceFormat)
    {
        IFC(m_pFactory->CreateFormatConverter(&pConverter));

        IFC(pConverter->Initialize(m_pSource, format, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut));

        *ppSource = pConverter;
        pConverter = NULL;
    }
    else
    {
        SetObject(*ppSource, m_pSource);
    }

Cleanup:
    ReleaseObject(pConverter);

    return hr;
}

#endif