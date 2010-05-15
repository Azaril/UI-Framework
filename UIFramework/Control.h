#pragma once

#include "FrameworkElement.h"
#include "ControlTemplate.h"
#include "Brush.h"

class CControl : public CFrameworkElement
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Control, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        //
        // Properties
        //
        static CStaticProperty TemplateProperty;
        static CStaticProperty BackgroundProperty;
        static CStaticProperty BorderBrushProperty;

    protected:
        CControl();
        virtual ~CControl();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual CUIElement* GetTemplateParentForChildren();

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnTemplateChanged );

        HRESULT OnTemplateChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetEffectiveTemplate( CControlTemplate** ppTemplate );
        
        HRESULT RevokeTemplate();
        HRESULT EnsureTemplate();
        HRESULT ApplyTemplate( CControlTemplate* pTemplate );

        HRESULT GetTemplateChild( CUIElement** ppChild );

    private:
        CTypedLayeredValue< CControlTemplate > m_Template;
        CTypedLayeredValue< CBrush > m_Background;
        CTypedLayeredValue< CBrush > m_BorderBrush;
        BOOL m_TemplateDirty;
        CUIElement* m_TemplateChild;
};

template< >
struct ObjectTypeTraits< CControl >
{
    static const TypeIndex::Value Type = TypeIndex::Control;
};