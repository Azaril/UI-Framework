#include "BitmapBase.h"

CBitmapBase::CBitmapBase(
    )
    : m_pTexture(NULL)
{
}

CBitmapBase::~CBitmapBase(
    )
{
    ReleaseObject(m_pTexture);
}

__checkReturn HRESULT
CBitmapBase::Initialize(
    __in ITexture* pTexture
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pTexture, pTexture);

    return hr;
}

__out ITexture*
CBitmapBase::GetTexture(
    )
{
    return m_pTexture;
}