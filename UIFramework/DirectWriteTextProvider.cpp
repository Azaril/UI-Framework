#include "DirectWriteTextProvider.h"
#include "DirectWriteTextFormat.h"
#include "DirectWriteTextLayout.h"
#include "DirectWriteEditableTextLayout.h"

typedef HRESULT (WINAPI *DWriteCreateFactoryFunc)(
	__in DWRITE_FACTORY_TYPE factoryType, 
	__in REFIID iid, 
	__deref_out IUnknown** ppFactory 
	);

CDirectWriteTextProvider::CDirectWriteTextProvider(
	) 
	: m_DWriteModule(NULL)
	, m_Factory(NULL)
	, m_DefaultFormat(NULL)
{
}

CDirectWriteTextProvider::~CDirectWriteTextProvider(
	)
{
    ReleaseObject(m_DefaultFormat);
    ReleaseObject(m_Factory);

    if(m_DWriteModule)
    {
        FreeLibrary(m_DWriteModule);
    }
}

__checkReturn HRESULT 
CDirectWriteTextProvider::Initialize(
	)
{
    HRESULT hr = S_OK;
    DWriteCreateFactoryFunc CreateFactory = NULL;

    m_DWriteModule = LoadLibrary(L"dwrite.dll");
    IFCEXPECT(m_DWriteModule != NULL);

    CreateFactory = (DWriteCreateFactoryFunc)GetProcAddress(m_DWriteModule, "DWriteCreateFactory");
    IFCPTR(CreateFactory);

    IFC(CreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_Factory));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteTextProvider::CreateFormat(
	__in const CFontDescription* pFontDescription,
	__deref_out CTextFormat** ppTextFormat
	)
{
    HRESULT hr = S_OK;
    IDWriteTextFormat* pDirectWriteTextFormat = NULL;
    CDirectWriteTextFormat* pFormat = NULL;

    IFC(m_Factory->CreateTextFormat(pFontDescription->GetFontName(), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, pFontDescription->GetFontSize(), pFontDescription->GetLocale(), &pDirectWriteTextFormat));

    IFC(CDirectWriteTextFormat::Create(pDirectWriteTextFormat, &pFormat));

    *ppTextFormat = pFormat;
    pFormat = NULL;

Cleanup:
    ReleaseObject(pDirectWriteTextFormat);
    ReleaseObject(pFormat);

    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteTextProvider::CreateTextLayout(
	__in_ecount(CharacterCount) const WCHAR* pText, 
	UINT32 CharacterCount, 
	__in CTextFormat* pTextFormat, 
	const SizeF& Size, 
	__deref_out CTextLayout** ppTextLayout
	)
{
    HRESULT hr = S_OK;
    CDirectWriteTextFormat* pDirectWriteTextFormat = NULL;
    CDirectWriteTextLayout* pDirectWriteTextLayout = NULL;
    IDWriteTextLayout* pTextLayout = NULL;

    IFCPTR(pText);
    IFCPTR(pTextFormat);
    IFCPTR(ppTextLayout);

    pDirectWriteTextFormat = (CDirectWriteTextFormat*)pTextFormat;

    IFC(m_Factory->CreateTextLayout(pText, CharacterCount, pDirectWriteTextFormat->GetDirectWriteTextFormat(), Size.width, Size.height, &pTextLayout));

    IFC(CDirectWriteTextLayout::Create(pTextLayout, &pDirectWriteTextLayout));

    *ppTextLayout = pDirectWriteTextLayout;
    pDirectWriteTextLayout = NULL;

Cleanup:
    ReleaseObject(pDirectWriteTextLayout);
    ReleaseObject(pTextLayout);

    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteTextProvider::CreateEditableTextLayout(
    __in CTextFormat* pTextFormat, 
	const SizeF& Size, 
	__deref_out CEditableTextLayout** ppEditableTextLayout
	)
{
    HRESULT hr = S_OK;
    CDirectWriteTextFormat* pDirectWriteTextFormat = NULL;
    CDirectWriteEditableTextLayout* pDirectWriteEditableTextLayout = NULL;

    IFCPTR(ppEditableTextLayout);

    pDirectWriteTextFormat = (CDirectWriteTextFormat*)pTextFormat;

    IFC(CDirectWriteEditableTextLayout::Create(this, pDirectWriteTextFormat, Size, &pDirectWriteEditableTextLayout));

    *ppEditableTextLayout = pDirectWriteEditableTextLayout;
    pDirectWriteEditableTextLayout = NULL;

Cleanup:
    ReleaseObject(pDirectWriteEditableTextLayout);

    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteTextProvider::RegisterFont(
    __in IResourceProvider* pResourceProvider,
    __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
    UINT32 IdentifierLength
    )
{
    HRESULT hr = S_OK;

    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}