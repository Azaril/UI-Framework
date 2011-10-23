#pragma once

#include "EditableTextLayout.h"
#include "Factory.h"
#include "TextProvider.h"
#include <string>

#include <dwrite.h>

class CDirectWriteEditableTextLayout : public CEditableTextLayout
{
    public:
        DECLARE_FACTORY3( CDirectWriteEditableTextLayout, CTextProvider*, CTextFormat*, const SizeF& );

        __checkReturn HRESULT GetDirectWriteTextLayout(
			__deref_out IDWriteTextLayout** ppLayout 
			);

        __override virtual __checkReturn HRESULT SetMaxSize(
			const SizeF& Size 
			);

        __override virtual __checkReturn HRESULT GetMetrics(
			__deref_out CTextLayoutMetrics** ppMetrics 
			);

        __override virtual UINT32 GetStartPosition(
			);

        __override virtual UINT32 GetEndPosition(
			);

        __override virtual __checkReturn HRESULT SetText(
			__in_ecount_opt(TextLength) const WCHAR* pText, 
			UINT32 TextLength 
			);

        __override virtual __checkReturn HRESULT ClearText(
			);

        __override virtual __checkReturn HRESULT InsertText(
			UINT32 Position, 
			__in_ecount(TextLength) const WCHAR* pText, 
			UINT32 TextLength 
			);

        __override virtual __checkReturn HRESULT RemoveText(
			UINT32 Position, 
			UINT32 Length 
			);

        __override virtual __checkReturn HRESULT GetText(
            __deref_out_ecount(*pTextLength) const WCHAR** ppText,
            __out UINT32* pTextLength
			);

    protected:
        CDirectWriteEditableTextLayout(
			);

        virtual ~CDirectWriteEditableTextLayout(
			);

        __checkReturn HRESULT Initialize( 
			__in CTextProvider* pTextProvider,
            __in CTextFormat* pFormat,
			const SizeF& MaxSize 
			);

        __checkReturn HRESULT InvalidateLayout(
			);

        __checkReturn HRESULT EnsureLayout(
			);

        CTextProvider* m_TextProvider;
        std::wstring m_Text;
        CTextLayout* m_TextLayout;
        CTextFormat* m_TextFormat;
        SizeF m_MaxSize;
};
