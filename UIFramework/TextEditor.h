#pragma once

#include "UIElement.h"
#include "EditableTextLayout.h"

class CTextEditor : public CRefCountedObject
{
    public:
        DECLARE_FACTORY( CTextEditor );

        HRESULT SetTextHost( CUIElement* pElement );
        HRESULT SetTextLayout( CEditableTextLayout* pLayout );
        HRESULT SetText( const WCHAR* pText, UINT32 TextLength );

        const WCHAR* GetText();

        void SetAcceptsEnter( BOOL AcceptsEnter );

    protected:
        CTextEditor();
        virtual ~CTextEditor();

        HRESULT Initialize();

        void OnText( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        void OnKeyDown( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );
        void OnKeyUp( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        HRESULT InsertText( UINT32 StartPosition, const WCHAR* pText, UINT32 TextLength );
        HRESULT RemoveText( UINT32 StartPosition, UINT32 Length );

        HRESULT HandleText( const WCHAR* pText, UINT32 TextLength );
        HRESULT HandleBackspace( BOOL* pConsumed );
        HRESULT HandleEnter( BOOL* pConsumed );

        BOOL HasSelection();
        HRESULT DeleteSelection();

        CUIElement* m_Host;
        CEditableTextLayout* m_Layout;
        events::signals::connection m_TextConnection;
        events::signals::connection m_KeyDownConnection;
        events::signals::connection m_KeyUpConnection;
        UINT32 m_CaretPosition;
        std::wstring m_TextHolder;
        BOOL m_AcceptsEnter;
};
