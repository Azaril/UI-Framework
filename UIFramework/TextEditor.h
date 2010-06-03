#pragma once

#include "UIElement.h"
#include "EditableTextLayout.h"

class CTextEditor : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CTextEditor, CUIElement*, CEditableTextLayout* );

        HRESULT SetText( const WCHAR* pText, UINT32 TextLength );

    protected:
        CTextEditor();
        virtual ~CTextEditor();

        HRESULT Initialize( CUIElement* pElement, CEditableTextLayout* pLayout );

        void OnText( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        void OnKeyDown( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        void OnKeyUp( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        HRESULT InsertText( UINT32 StartPosition, const WCHAR* pText, UINT32 TextLength );
        HRESULT RemoveText( UINT32 StartPosition, UINT32 Length );

        HRESULT HandleText( const WCHAR* pText, UINT32 TextLength );
        HRESULT HandleBackspace();

        BOOL HasSelection();
        HRESULT DeleteSelection();

        CUIElement* m_Host;
        CEditableTextLayout* m_Layout;
        events::signals::connection m_TextConnection;
        events::signals::connection m_KeyDownConnection;
        events::signals::connection m_KeyUpConnection;
        UINT32 m_CaretPosition;
};
