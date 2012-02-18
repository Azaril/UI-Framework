#pragma once

#include "Control.h"
#include "TextEditor.h"
#include "TextHost.h"
#include "ContentPresenter.h"
#include "LayeredValue.h"

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
        static CStaticProperty AcceptsReturnProperty;

    protected:
        CTextBox();
        virtual ~CTextBox();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual HRESULT PostTemplateApplied();
        virtual HRESULT PreTemplateRevoked();

        __checkReturn HRESULT EnsureTextLayout(
            );

        __checkReturn HRESULT GetTextLayout(
            __deref_out CEditableTextLayout** ppTextLayout
            );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnTextChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnAcceptsReturnChanged );

        HRESULT OnTextChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnAcceptsReturnChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CEditableTextLayout* m_TextLayout;
        CTextFormat* m_TextFormat;
        CTextEditor* m_TextEditor;
        CTextHost* m_TextHost;
        CContentPresenter* m_TextHostControl;
        CLayeredValue m_AcceptsReturn;
};

template< >
struct ObjectTypeTraits< CTextBox >
{
    static const TypeIndex::Value Type = TypeIndex::TextBox;
};