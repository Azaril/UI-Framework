#pragma once

#include "TextLayout.h"

class CEditableTextLayout : public CTextLayout
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::EditableTextLayout, CTextLayout );

        virtual UINT32 GetStartPosition(
			) = 0;

        virtual UINT32 GetEndPosition(
			) = 0;

        virtual __checkReturn HRESULT SetText( 
			__in_ecount_opt(TextLength) const WCHAR* pText, 
			UINT32 TextLength 
			) = 0;

        virtual __checkReturn HRESULT ClearText(
			) = 0;

        virtual __checkReturn HRESULT InsertText(
			UINT32 Position, 
			__in_ecount(TextLength) const WCHAR* pText,
			UINT32 TextLength
			) = 0;

        virtual __checkReturn HRESULT RemoveText( 
			UINT32 Position, 
			UINT32 Length 
			) = 0;

        virtual __checkReturn HRESULT GetText(
            __deref_out_ecount(*pTextLength) const WCHAR** ppText,
            __out UINT32* pTextLength
            ) = 0;
};
