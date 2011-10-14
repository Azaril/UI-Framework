#include "StagingTextureWrapper.h"

CStagingTextureWrapper::CStagingTextureWrapper(
    )
    : m_pAllocator(NULL)
    , m_pCallback(NULL)
    , m_pTargetTexture(NULL)
{
}

CStagingTextureWrapper::~CStagingTextureWrapper(
    )
{
    ReleaseObject(m_pTargetTexture);
}

__checkReturn HRESULT
CStagingTextureWrapper::Initialize(
    __in ITextureAllocator* pAllocator,
    __in IStagingTextureCallback* pCallback,
    __in ITexture* pTargetTexture
    )
{
    HRESULT hr = S_OK;

    m_pAllocator = pAllocator;
    m_pCallback = pCallback;
    SetObject(m_pTargetTexture, pTargetTexture);

    return hr;
}

__out ITexture*
CStagingTextureWrapper::GetTargetTexture(
    )
{
    return m_pTargetTexture;
}

__override UINT32 
CStagingTextureWrapper::GetWidth(
    )
{
    return m_pTargetTexture->GetWidth();
}

__override UINT32 
CStagingTextureWrapper::GetHeight(
    )
{
    return m_pTargetTexture->GetHeight();
}

__override PixelFormat::Value 
CStagingTextureWrapper::GetPixelFormat(
    )
{
    return m_pTargetTexture->GetPixelFormat();
}

__override __checkReturn HRESULT 
CStagingTextureWrapper::SetData(
    __in_ecount(DataSize) BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__override __checkReturn HRESULT 
CStagingTextureWrapper::SetSubData(
    const RectU& Region,
    __in_ecount(DataSize) BYTE* pData,
    UINT32 DataSize,
    INT32 Stride
    )
{
    HRESULT hr = S_OK;
    ITexture* pStagingTexture = NULL;
    UINT32 width = Region.GetWidth();
    UINT32 height = Region.GetHeight();

    IFC(m_pAllocator->AllocateTexture(width, height, &pStagingTexture));

    IFC(pStagingTexture->SetData(pData, DataSize, Stride));

    IFC(m_pCallback->AddUpdate(pStagingTexture, MakeRect(SizeU(width, height)), m_pTargetTexture, Region.GetTopLeft()));

Cleanup:
    ReleaseObject(pStagingTexture);

    return hr;
}