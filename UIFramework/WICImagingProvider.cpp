#include "WICImagingProvider.h"
#include "COMReadStreamWrapper.h"

EXTERN_C const GUID DECLSPEC_SELECTANY CLSID_WICImagingFactory = { 0xcacaf262, 0x9370, 0x4615, { 0xa1, 0x3b,  0x9f,  0x55,  0x39,  0xda,  0x4c,  0xa } };
EXTERN_C const GUID DECLSPEC_SELECTANY GUID_WICPixelFormat32bppPBGRA = { 0x6fddc324, 0x4e03, 0x4bfe, { 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x10 } };

CWICImagingProvider::CWICImagingProvider(
	) 
	: m_Factory(NULL)
	, m_UninitializeCOM(FALSE)
	, m_ShlwapiModule(NULL)
	, m_SHCreateMemStream(NULL)
{
}

CWICImagingProvider::~CWICImagingProvider(
	)
{
    ReleaseObject(m_Factory);

    if(m_UninitializeCOM)
    {
        CoUninitialize();
    }
}

__checkReturn HRESULT 
CWICImagingProvider::Initialize(
	)
{
    HRESULT hr = S_OK;

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (hr == RPC_E_CHANGED_MODE)
    {
        hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    }

    IFC(hr);

    m_UninitializeCOM = TRUE;

    IFC(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void**)&m_Factory));

    m_ShlwapiModule = LoadLibrary(L"Shlwapi.dll");
    IFCEXPECT(m_ShlwapiModule != NULL);

    m_SHCreateMemStream = (SHCreateMemStreamFunc)GetProcAddress(m_ShlwapiModule, "SHCreateMemStream");
    IFCPTR(m_SHCreateMemStream);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CWICImagingProvider::LoadBitmapFromDecoder(
	__in IWICBitmapDecoder* pDecoder, 
	__deref_out CWICBitmapSource** ppBitmapSource
	)
{
    HRESULT hr = S_OK;
    CWICBitmapSource* pWICBitmapSource = NULL;
    IWICBitmapFrameDecode* pSource = NULL;
    IWICFormatConverter* pConverter = NULL;

    IFCPTR(pDecoder);

    IFC(pDecoder->GetFrame(0, &pSource));

    IFC(CWICBitmapSource::Create(m_Factory, pSource, &pWICBitmapSource));

    *ppBitmapSource = pWICBitmapSource;
    pWICBitmapSource = NULL;

Cleanup:
    ReleaseObject(pSource);
    ReleaseObject(pConverter);
    ReleaseObject(pWICBitmapSource);

    return hr;
}

__override __checkReturn HRESULT 
CWICImagingProvider::LoadBitmapFromFile(
	__in_z const WCHAR* pPath, 
	__deref_out CBitmapSource** ppBitmapSource
	)
{
    HRESULT hr = S_OK;
    CWICBitmapSource* pBitmapSource = NULL;
    IWICBitmapDecoder* pDecoder = NULL;

    IFCPTR(pPath);
    IFCPTR(ppBitmapSource);

    IFC(m_Factory->CreateDecoderFromFilename(pPath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder));

    IFC(LoadBitmapFromDecoder(pDecoder, &pBitmapSource));

    *ppBitmapSource = pBitmapSource;
    pBitmapSource = NULL;

Cleanup:
    ReleaseObject(pBitmapSource);
    ReleaseObject(pDecoder);

    return hr;
}

__override __checkReturn HRESULT 
CWICImagingProvider::LoadBitmapFromMemory(
	__in_bcount(DataSize) const BYTE* pData, 
	UINT32 DataSize,
	__deref_out CBitmapSource** ppBitmapSource
	)
{
    HRESULT hr = S_OK;
    CWICBitmapSource* pBitmapSource = NULL;
    IWICBitmapDecoder* pDecoder = NULL;
    IStream* pStream = NULL;

    IFCPTR(pData);
    IFCPTR(ppBitmapSource);

    pStream = m_SHCreateMemStream(pData, DataSize);
    IFCPTR(pStream);

    IFC(m_Factory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder));

    IFC(LoadBitmapFromDecoder(pDecoder, &pBitmapSource));

    IFC(pBitmapSource->AssociateStream(pStream));

    *ppBitmapSource = pBitmapSource;
    pBitmapSource = NULL;

Cleanup:
    ReleaseObject(pStream);
    ReleaseObject(pBitmapSource);
    ReleaseObject(pDecoder);

    return hr;
}

__override __checkReturn HRESULT 
CWICImagingProvider::LoadBitmapFromStream(
    __in IReadStream* pStream,
    __deref_out CBitmapSource** ppBitmapSource
    )
{
    HRESULT hr = S_OK;
    CCOMReadStreamWrapper* pCOMStream = NULL;
    IWICBitmapDecoder* pDecoder = NULL;
    CWICBitmapSource* pBitmapSource = NULL;

    IFC(CCOMReadStreamWrapper::Create(pStream, &pCOMStream));

    IFC(m_Factory->CreateDecoderFromStream(pCOMStream, NULL, WICDecodeMetadataCacheOnDemand, &pDecoder));

    IFC(LoadBitmapFromDecoder(pDecoder, &pBitmapSource));

    IFC(pBitmapSource->AssociateStream(pCOMStream));

    *ppBitmapSource = pBitmapSource;
    pBitmapSource = NULL;

Cleanup:
    ReleaseObject(pCOMStream);
    ReleaseObject(pDecoder);
    ReleaseObject(pBitmapSource);

    return hr;
}