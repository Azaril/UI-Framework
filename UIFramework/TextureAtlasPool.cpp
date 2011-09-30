#include "TextureAtlasPool.h"

CTextureAtlasPool::CTextureAtlasPool(
	)
	: m_Width(0)
	, m_Height(0)
    , m_Padding(0)
	, m_pTextureAllocator(NULL)
{
}

CTextureAtlasPool::~CTextureAtlasPool(
	)
{
	for (std::vector< CTextureAtlas* >::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it)
	{
		(*it)->Release();
	}
}

__checkReturn HRESULT
CTextureAtlasPool::Initialize(
	UINT32 Width,
	UINT32 Height,
    UINT32 Padding,
	__in ITextureAllocator* pAllocator
	)
{
	HRESULT hr = S_OK;

	m_Width = Width;
	m_Height = Height;
    m_Padding = Padding;
	m_pTextureAllocator = pAllocator;

	return hr;
}

__override __checkReturn HRESULT
CTextureAtlasPool::AllocateTexture(
	UINT32 Width,
	UINT32 Height,
	__deref_out ITexture** ppTexture
	)
{
	HRESULT hr = S_OK;
	ITexture* pNewAtlasStorageTexture = NULL;
	CTextureAtlas* pNewAtlas = NULL;

    //TODO: Implement a better algorithm here, this will get slow as more textures get allocated and fragmentation happens.
	for (std::vector< CTextureAtlas* >::reverse_iterator it = m_Textures.rbegin(); it != m_Textures.rend(); ++it)
	{
		CTextureAtlas* pAtlas = (*it);

		if(SUCCEEDED(pAtlas->AllocateTexture(Width, Height, ppTexture)))
		{
			goto Cleanup;
		}
	}

	IFC(m_pTextureAllocator->AllocateTexture(m_Width, m_Height, &pNewAtlasStorageTexture));

	IFC(CTextureAtlas::Create(pNewAtlasStorageTexture, &pNewAtlas));

	IFC(pNewAtlas->AllocateTexture(Width, Height, ppTexture));

	m_Textures.push_back(pNewAtlas);
	pNewAtlas = NULL;

Cleanup:
	ReleaseObject(pNewAtlasStorageTexture);

	return hr;
}