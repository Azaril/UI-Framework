#include "DirectWriteTextProvider.h"
#include "DirectWriteTextFormat.h"
#include "DirectWriteTextLayout.h"
#include "DirectWriteEditableTextLayout.h"
#include "StackHeapBuffer.h"

typedef HRESULT (WINAPI *DWriteCreateFactoryFunc)(
	__in DWRITE_FACTORY_TYPE factoryType, 
	__in REFIID iid, 
	__deref_out IUnknown** ppFactory 
	);

CDirectWriteTextProvider::CDirectWriteTextProvider(
	) 
	: m_DWriteModule(NULL)
	, m_Factory(NULL)
    , m_pFontCollectionLoader(NULL)
    , m_pRegisteredFonts(NULL)
    , m_pFontFileLoader(NULL)
{
}

CDirectWriteTextProvider::~CDirectWriteTextProvider(
	)
{
    for (vector< IDWriteFontCollection* >::iterator it = m_FontCollections.begin(); it != m_FontCollections.end(); ++it)
    {
        (*it)->Release();
    }

    if (m_Factory != NULL)
    {
        if (m_pFontFileLoader != NULL)
        {
            IGNOREHR(m_Factory->UnregisterFontFileLoader(m_pFontFileLoader));
        }

        if (m_pFontCollectionLoader != NULL)
        {
            IGNOREHR(m_Factory->UnregisterFontCollectionLoader(m_pFontCollectionLoader));
        }
    }

    ReleaseObject(m_pRegisteredFonts);
    ReleaseObject(m_pFontFileLoader);
    ReleaseObject(m_pFontCollectionLoader);
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

    IFC(CDirectWriteRegisteredFontCollection::Create(&m_pRegisteredFonts));

    IFC(CDirectWriteFontFileLoader::Create(m_pRegisteredFonts, &m_pFontFileLoader));

    IFC(CDirectWriteFontCollectionLoader::Create(m_pFontFileLoader, &m_pFontCollectionLoader));

    IFC(m_Factory->RegisterFontFileLoader(m_pFontFileLoader));

    IFC(m_Factory->RegisterFontCollectionLoader(m_pFontCollectionLoader));

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
    IDWriteFontFamily* pFontFamily = NULL;
    IDWriteLocalizedStrings* pFamilyNames = NULL;
    IDWriteFontCollection* pFontCollection = NULL;
    StackHeapBuffer< WCHAR, 256 > nameBuffer;

    //
    // Find the collection that contains the specified name.
    //
    for (vector< IDWriteFontCollection* >::iterator it = m_FontCollections.begin(); pFontCollection == NULL && it != m_FontCollections.end(); ++it)
    {
        UINT32 fontFamilyCount = (*it)->GetFontFamilyCount();
        
        //
        // Search each font family contained in the collection.
        //
        for (UINT32 i = 0; pFontCollection == NULL && i < fontFamilyCount; ++i)
        {
            BOOL hasLocale = FALSE;
            UINT32 localeIndex = 0;
            UINT32 searchStartIndex = 0;
            UINT32 searchEndIndex = 0;

            IFC((*it)->GetFontFamily(i, &pFontFamily));

            IFC(pFontFamily->GetFamilyNames(&pFamilyNames));

            //
            // Attempt to find the localized string.
            //
            IFC(pFamilyNames->FindLocaleName(pFontDescription->GetLocale(), &localeIndex, &hasLocale));

            //
            // Use the locale specific name otherwise search all locales.
            //
            if (hasLocale)
            {
                searchStartIndex = localeIndex;
                searchEndIndex = localeIndex + 1;
            }
            else
            {
                searchStartIndex = 0;
                searchEndIndex = pFamilyNames->GetCount();
            }

            //
            // Search localized string range for family name.
            //
            for (UINT32 j = searchStartIndex; j < searchEndIndex; ++j)
            {
                UINT32 stringSize = 0;

                IFC(pFamilyNames->GetStringLength(j, &stringSize));

                IFC(nameBuffer.EnsureBufferSize(stringSize + 1));

                IFC(pFamilyNames->GetString(localeIndex, nameBuffer.GetBuffer(), nameBuffer.GetBufferSize()));

                //
                // Test if family name matches.
                //
                if (wcscmp(nameBuffer.GetBuffer(), pFontDescription->GetFontName()) == 0)
                {
                    //
                    // Found the correct font collection to load the font from.
                    //
                    pFontCollection = (*it);

                    break;
                }
            }

            ReleaseObject(pFontFamily);
            ReleaseObject(pFamilyNames);
        }
    }

    IFCPTR(pFontCollection);

    IFC(m_Factory->CreateTextFormat(pFontDescription->GetFontName(), pFontCollection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, pFontDescription->GetFontSize(), pFontDescription->GetLocale(), &pDirectWriteTextFormat));

    IFCPTR(pDirectWriteTextFormat);

    IFC(CDirectWriteTextFormat::Create(pDirectWriteTextFormat, &pFormat));

    *ppTextFormat = pFormat;
    pFormat = NULL;

Cleanup:
    ReleaseObject(pDirectWriteTextFormat);
    ReleaseObject(pFormat);
    ReleaseObject(pFontFamily);
    ReleaseObject(pFamilyNames);

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
    IDWriteFontCollection* pCollection = NULL;
    UINT32 fontToken = 0;

    IFC(m_pRegisteredFonts->RegisterFont(pResourceProvider, pIdentifier, IdentifierLength, &fontToken));

    IFC(m_Factory->CreateCustomFontCollection(m_pFontCollectionLoader, &fontToken, sizeof(fontToken), &pCollection));

    m_FontCollections.push_back(pCollection);

    pCollection = NULL;

Cleanup:
    ReleaseObject(pCollection);

    return hr;
}