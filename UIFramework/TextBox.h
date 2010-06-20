#pragma once

#include "Control.h"
#include "TextEditor.h"
#include "TextHost.h"
#include "ContentPresenter.h"

class CTextBox : public CControl
{
    public:
        DECLARE_FACTORY1( CTextBox, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TextBox, CControl );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Properties
        //
        static CStaticProperty TextProperty;

    protected:
        CTextBox();
        virtual ~CTextBox();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnTextChanged );

        HRESULT OnTextChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CEditableTextLayout* m_TextLayout;
        CTextEditor* m_TextEditor;
        CTextHost* m_TextHost;
        CTypedLayeredValue< CStringValue > m_Text;
        CContentPresenter* m_TextHostControl;
};

template< >
struct ObjectTypeTraits< CTextBox >
{
    static const TypeIndex::Value Type = TypeIndex::TextBox;
};