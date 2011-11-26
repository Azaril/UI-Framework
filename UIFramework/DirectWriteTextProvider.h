#pragma once

#include "TextProvider.h"
#include "D2DGraphicsDevice.h"
#include "DirectWriteTextFormat.h"
#include "DirectWriteFontCollectionLoader.h"
#include "DirectWriteRegisteredFontCollection.h"
#include "DirectWriteFontFileLoader.h"

#include <dwrite.h>

class CDirectWriteTextProvider : public CTextProvider
{
    public:
        DECLARE_FACTORY( CDirectWriteTextProvider );

        __override virtual __checkReturn HRESULT CreateFormat(
			__in const CFontDescription* pFontDescription,
			__deref_out CTextFormat** ppTextFormat 
			);

        __override virtual __checkReturn HRESULT CreateTextLayout(
			__in_ecount(CharacterCount) const WCHAR* pText, 
			UINT32 CharacterCount, 
			__in CTextFormat* pTextFormat, 
			const SizeF& Size,
			__deref_out CTextLayout** ppTextLayout 
			);

        __override virtual __checkReturn HRESULT CreateEditableTextLayout(
            __in CTextFormat* pTextFormat,
			const SizeF& Size, 
			__deref_out CEditableTextLayout** ppEditableTextLayout 
			);

        __override virtual __checkReturn HRESULT RegisterFont(
            __in IResourceProvider* pResourceProvider,
            __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
            UINT32 IdentifierLength
            );

    protected:
        CDirectWriteTextProvider(
			);

        virtual ~CDirectWriteTextProvider(
			);

        __checkReturn HRESULT Initialize(
			);

        HMODULE m_DWriteModule;
        IDWriteFactory* m_Factory;
        CDirectWriteRegisteredFontCollection* m_pRegisteredFonts;
        CDirectWriteFontCollectionLoader* m_pFontCollectionLoader;
        CDirectWriteFontFileLoader* m_pFontFileLoader;
        vector< IDWriteFontCollection* > m_FontCollections;
};
