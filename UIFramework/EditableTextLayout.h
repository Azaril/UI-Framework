#pragma once

#include "TextLayout.h"

class CEditableTextLayout : public CTextLayout
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::EditableTextLayout, CTextLayout );

        virtual UINT32 GetStartPosition() = 0;
        virtual UINT32 GetEndPosition() = 0;

        virtual HRESULT SetText( const WCHAR* pText, UINT32 TextLength ) = 0;
        virtual HRESULT ClearText() = 0;
        virtual HRESULT InsertText( UINT32 Position, const WCHAR* pText, UINT32 TextLength ) = 0;
        virtual HRESULT RemoveText( UINT32 Position, UINT32 Length ) = 0;

        virtual const WCHAR* GetText() = 0;
};
