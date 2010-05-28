#pragma once

#include "UIElement.h"
#include "EditableTextLayout.h"

class CTextEditor : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CTextEditor, CUIElement*, CEditableTextLayout* );

    protected:
        CTextEditor();
        virtual ~CTextEditor();

        HRESULT Initialize( CUIElement* pElement, CEditableTextLayout* pLayout );

        void OnText( CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs );

        HRESULT InsertText( const WCHAR* pText, UINT32 TextLength );

        CUIElement* m_Host;
        CEditableTextLayout* m_Layout;
        connection m_TextConnection;
};
