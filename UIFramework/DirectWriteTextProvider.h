#pragma once

#include "TextProvider.h"
#include "D2DGraphicsDevice.h"
#include "DirectWriteTextFormat.h"

#include <dwrite.h>

class CDirectWriteTextProvider : public CTextProvider
{
    public:
        DECLARE_FACTORY( CDirectWriteTextProvider );

        virtual HRESULT CreateFormat( const WCHAR* pFontName, FLOAT FontSize, const WCHAR* pLocaleName, CTextFormat** ppTextFormat );
        virtual HRESULT GetDefaultFormat( CTextFormat** ppTextFormat );

        virtual HRESULT CreateTextLayout( const WCHAR* pText, UINT32 CharacterCount, CTextFormat* pTextFormat, const SizeF& Size, CTextLayout** ppTextLayout );

    protected:
        CDirectWriteTextProvider();
        virtual ~CDirectWriteTextProvider();

        HRESULT Initialize();

        HMODULE m_DWriteModule;
        IDWriteFactory* m_Factory;
        CTextFormat* m_DefaultFormat;
};
