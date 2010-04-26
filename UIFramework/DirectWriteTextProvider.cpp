#include "DirectWriteTextProvider.h"
#include "DirectWriteTextFormat.h"
#include "DirectWriteTextLayout.h"

typedef HRESULT (WINAPI *DWriteCreateFactoryFunc)( __in DWRITE_FACTORY_TYPE factoryType, __in REFIID iid, __out IUnknown **factory );

CDirectWriteTextProvider::CDirectWriteTextProvider() : m_DWriteModule(NULL),
                                                       m_Factory(NULL),
                                                       m_DefaultFormat(NULL)
{
}

CDirectWriteTextProvider::~CDirectWriteTextProvider()
{
    ReleaseObject(m_DefaultFormat);
    ReleaseObject(m_Factory);

    if(m_DWriteModule)
    {
        FreeLibrary(m_DWriteModule);
    }
}

HRESULT CDirectWriteTextProvider::Initialize()
{
    HRESULT hr = S_OK;
    DWriteCreateFactoryFunc CreateFactory = NULL;

    m_DWriteModule = LoadLibrary(L"dwrite.dll");
    IFCEXPECT(m_DWriteModule != NULL);

    CreateFactory = (DWriteCreateFactoryFunc)GetProcAddress(m_DWriteModule, "DWriteCreateFactory");
    IFCPTR(CreateFactory);

    IFC(CreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_Factory));

    IFC(CreateFormat(L"Arial", 12, L"en-us", &m_DefaultFormat));

Cleanup:
    return hr;
}

HRESULT CDirectWriteTextProvider::CreateFormat(const WCHAR* pFontName, FLOAT FontSize, const WCHAR* pLocaleName, CTextFormat** ppTextFormat)
{
    HRESULT hr = S_OK;
    IDWriteTextFormat* pDirectWriteTextFormat = NULL;
    CDirectWriteTextFormat* pFormat = NULL;

    IFCPTR(pFontName);
    IFCPTR(pLocaleName);
    IFCPTR(ppTextFormat);

    IFC(m_Factory->CreateTextFormat(pFontName, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, FontSize, pLocaleName, &pDirectWriteTextFormat));

    IFC(CDirectWriteTextFormat::Create(pDirectWriteTextFormat, &pFormat));

    *ppTextFormat = pFormat;
    pFormat = NULL;

Cleanup:
    ReleaseObject(pDirectWriteTextFormat);
    ReleaseObject(pFormat);

    return hr;
}

HRESULT CDirectWriteTextProvider::GetDefaultFormat(CTextFormat** ppTextFormat)
{
    HRESULT hr = S_OK;

    IFCPTR(ppTextFormat);

    *ppTextFormat = m_DefaultFormat;
    AddRefObject(m_DefaultFormat);

Cleanup:
    return hr;
}

HRESULT CDirectWriteTextProvider::CreateTextLayout(const WCHAR* pText, UINT32 CharacterCount, CTextFormat* pTextFormat, const SizeF& Size, CTextLayout** ppTextLayout)
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