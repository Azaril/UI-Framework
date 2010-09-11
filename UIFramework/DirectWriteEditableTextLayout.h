#pragma once

#include "EditableTextLayout.h"
#include "Factory.h"
#include "TextProvider.h"
#include <string>

class CDirectWriteEditableTextLayout : public CEditableTextLayout
{
    public:
        DECLARE_FACTORY2( CDirectWriteEditableTextLayout, CTextProvider*, const SizeF& );

        HRESULT GetDirectWriteTextLayout( IDWriteTextLayout** ppLayout );

        virtual HRESULT SetMaxSize( const SizeF& Size );
        virtual HRESULT GetMetrics( CTextLayoutMetrics** ppMetrics );

        virtual UINT32 GetStartPosition();
        virtual UINT32 GetEndPosition();

        virtual HRESULT SetText( const WCHAR* pText, UINT32 TextLength );
        virtual HRESULT ClearText();
        virtual HRESULT InsertText( UINT32 Position, const WCHAR* pText, UINT32 TextLength );
        virtual HRESULT RemoveText( UINT32 Position, UINT32 Length );

        virtual const WCHAR* GetText();

    protected:
        CDirectWriteEditableTextLayout();
        virtual ~CDirectWriteEditableTextLayout();

        HRESULT Initialize( CTextProvider* pTextProvider, const SizeF& MaxSize );

        HRESULT InvalidateLayout();
        HRESULT EnsureLayout();

        CTextProvider* m_TextProvider;
        std::wstring m_Text;
        CTextLayout* m_TextLayout;
        CTextFormat* m_TextFormat;
        SizeF m_MaxSize;
};
