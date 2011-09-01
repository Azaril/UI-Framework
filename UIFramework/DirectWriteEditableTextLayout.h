#pragma once

#include "EditableTextLayout.h"
#include "Factory.h"
#include "TextProvider.h"
#include <string>

class CDirectWriteEditableTextLayout : public CEditableTextLayout
{
    public:
        DECLARE_FACTORY2( CDirectWriteEditableTextLayout, CTextProvider*, const SizeF& );

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

        __override virtual const WCHAR* GetText(
			);

    protected:
        CDirectWriteEditableTextLayout(
			);

        virtual ~CDirectWriteEditableTextLayout(
			);

        __checkReturn HRESULT Initialize( 
			__in CTextProvider* pTextProvider,
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
