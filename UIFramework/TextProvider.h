#pragma once

#include "TextFormat.h"
#include "TextLayout.h"

class CTextProvider : public CRefCountedObject
{
    public:
        virtual HRESULT CreateFormat( const WCHAR* pFontName, FLOAT FontSize, const WCHAR* pLocaleName, CTextFormat** ppTextFormat ) = 0;
        virtual HRESULT GetDefaultFormat( CTextFormat** ppTextFormat ) = 0;

        virtual HRESULT CreateTextLayout( const WCHAR* pText, UINT32 CharacterCount, CTextFormat* pTextFormat, const SizeF& Size, CTextLayout** ppTextLayout ) = 0;
};
