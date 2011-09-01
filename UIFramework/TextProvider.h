#pragma once

#include "TextFormat.h"
#include "TextLayout.h"
#include "EditableTextLayout.h"

class CTextProvider : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT CreateFormat( 
			__in_z const WCHAR* pFontName, 
			FLOAT FontSize, 
			__in_z const WCHAR* pLocaleName, 
			__deref_out CTextFormat** ppTextFormat 
			) = 0;

        virtual __checkReturn HRESULT GetDefaultFormat(
			__deref_out CTextFormat** ppTextFormat 
			) = 0;

        virtual __checkReturn HRESULT CreateTextLayout(
			__in_ecount(CharacterCount) const WCHAR* pText, 
			UINT32 CharacterCount, 
			__in CTextFormat* pTextFormat, 
			const SizeF& Size, 
			__deref_out CTextLayout** ppTextLayout
			) = 0;

        virtual __checkReturn HRESULT CreateEditableTextLayout( 
			const SizeF& Size, 
			__deref_out CEditableTextLayout** ppEditableTextLayout 
			) = 0;
};
