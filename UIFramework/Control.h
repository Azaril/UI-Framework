#pragma once

#include "FrameworkElement.h"
#include "ControlTemplate.h"

class CControl : public CFrameworkElement
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Control, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        //
        // Properties
        //
        static CStaticProperty TemplateProperty;

    protected:
        CControl();
        virtual ~CControl();

        HRESULT Initialize();

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnTemplateChanged );

        HRESULT OnTemplateChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetEffectiveTemplate( CControlTemplate** ppTemplate );
        
        HRESULT RevokeTemplate();
        HRESULT EnsureTemplate();
        HRESULT ApplyTemplate( CControlTemplate* pTemplate );

        CTypedLayeredValue< CControlTemplate > m_Template;
        BOOL m_TemplateDirty;
        CUIElement* m_TemplateChild;
};

template< >
struct ObjectTypeTraits< CControl >
{
    static const TypeIndex::Value Type = TypeIndex::Control;
};