#pragma once

#include "UIElement.h"
#include "EditableTextLayout.h"

class CTextEditor : public CRefCountedObject
{
    public:
        DECLARE_FACTORY( CTextEditor );

        __checkReturn HRESULT SetTextHost(
			__in_opt CUIElement* pElement 
			);

        __checkReturn HRESULT SetTextLayout(
			__in_opt CEditableTextLayout* pLayout 
			);

        __checkReturn HRESULT SetText( 
			__in_ecount_opt(TextLength) const WCHAR* pText, 
			UINT32 TextLength 
			);

        __out_opt const WCHAR* GetText(
			);

        void SetAcceptsEnter( 
			BOOL AcceptsEnter 
			);

    protected:
        CTextEditor(
			);

        virtual ~CTextEditor(
			);

        __checkReturn HRESULT Initialize(
			);

        void OnText( 
			__in CObjectWithType* pSender,
			__in CRoutedEventArgs* pRoutedEventArgs 
			);

        void OnKeyDown(
			__in CObjectWithType* pSender, 
			__in CRoutedEventArgs* pRoutedEventArgs 
			);

        void OnKeyUp(
			__in CObjectWithType* pSender, 
			__in CRoutedEventArgs* pRoutedEventArgs 
			);

        __checkReturn HRESULT InsertText(
			UINT32 StartPosition, 
			__in_ecount(TextLength) const WCHAR* pText, 
			UINT32 TextLength 
			);

        __checkReturn HRESULT RemoveText( 
			UINT32 StartPosition,
			UINT32 Length 
			);

        __checkReturn HRESULT HandleText(
			__in_ecount(TextLength) const WCHAR* pText, 
			UINT32 TextLength
			);

        __checkReturn HRESULT HandleBackspace(
			__out BOOL* pConsumed 
			);

        __checkReturn HRESULT HandleEnter(
			__out BOOL* pConsumed 
			);

        BOOL HasSelection(
			);

        __checkReturn HRESULT DeleteSelection(
			);

        CUIElement* m_Host;
        CEditableTextLayout* m_Layout;
        events::signals::connection m_TextConnection;
        events::signals::connection m_KeyDownConnection;
        events::signals::connection m_KeyUpConnection;
        UINT32 m_CaretPosition;
        std::wstring m_TextHolder;
        BOOL m_AcceptsEnter;
};
