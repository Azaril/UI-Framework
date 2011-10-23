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
    __in IResourceProvider* pResourceProvider,
	__deref_out CTextFormat** ppTextFormat
	)
{
    HRESULT hr = S_OK;
    IDWriteTextFormat* pDirectWriteTextFormat = NULL;
    CDirectWriteTextFormat* pFormat = NULL;
    //IDWriteFontFileLoader* pFontLoader = NULL;
    IDWriteFontFile* pFontFile = NULL;
    //IDWriteFontFace* pFontFace = NULL;

    //TODO: Add cache lookup for font.

    //TODO: Add cache lookup for font file.
    //IFC(GetFontFileLoaderForResourceProvider(pResourceProvider, &pFontLoader));

    //IFC(m_Factory->CreateCustomFontFileReference(pFontDescription->GetFontName(), wcslen(pFontDescription->GetFontName()), pFontLoader, &pFontFile));

    {
        BOOL supportedFont = FALSE;
        DWRITE_FONT_FILE_TYPE fontFileType;
        DWRITE_FONT_FACE_TYPE fontFaceType;
        UINT32 numberOfFaces = 0;

        IFC(pFontFile->Analyze(&supportedFont, &fontFileType, &fontFaceType, &numberOfFaces));

        IFCEXPECT(supportedFont);

        //IFC(m_Factory->CreateCustomFontCollection(
        //IFC(
    }
        //if (SUCCEEDED(m_Factory->CreateFontFace((DWRITE_FONT_FACE_TYPE)i, 1, &pFontFile, 0, DWRITE_FONT_SIMULATIONS_NONE, &pFontFace)))
        //{
        //    IFC(m_Factory->CreateTextFormat(
        //}
    //TODO: Load from resource provider.

    IFC(m_Factory->CreateTextFormat(pFontDescription->GetFontName(), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, pFontDescription->GetFontSize(), pFontDescription->GetLocale(), &pDirectWriteTextFormat));

    IFC(CDirectWriteTextFormat::Create(pDirectWriteTextFormat, &pFormat));

    *ppTextFormat = pFormat;
    pFormat = NULL;

Cleanup:
    ReleaseObject(pDirectWriteTextFormat);
    ReleaseObject(pFormat);
    ReleaseObject(pFontFile);

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