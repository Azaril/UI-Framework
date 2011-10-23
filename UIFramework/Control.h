#pragma once

#include "FrameworkElement.h"
#include "ControlTemplate.h"
#include "Brush.h"

class CControl : public CFrameworkElement
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Control, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT Measure( SizeF Size );

        virtual HRESULT Arrange( RectF Bounds );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        HRESULT GetTemplateChild( const WCHAR* pName, CObjectWithType** ppObject );

        template< typename T >
        HRESULT GetTemplateChild( const WCHAR* pName, T** ppObject )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pChildObject = NULL;
            T* pVal = NULL;

            IFCPTR(ppObject);

            IFC(GetTemplateChild(pName, &pChildObject));

            IFC(CastType(pChildObject, &pVal));

            *ppObject = pVal;
            pChildObject = NULL;
            pVal = NULL;

        Cleanup:
            ReleaseObject(pChildObject);

            return hr;
        }

        //
        // Properties
        //
        static CStaticProperty TemplateProperty;
        static CStaticProperty BackgroundProperty;
        static CStaticProperty BorderBrushProperty;
        static CStaticProperty BorderThicknessProperty;
        static CStaticProperty HorizontalContentAlignmentProperty;
        static CStaticProperty VerticalContentAlignmentProperty;

    protected:
        CControl();
        virtual ~CControl();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual CUIElement* GetTemplateParentForChildren();

        virtual HRESULT PostTemplateApplied();
        virtual HRESULT PreTemplateRevoked();

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
        CTypedLayeredValue< CControl, CControlTemplate > m_Template;
        CTypedLayeredValue< CControl, CBrush > m_Background;
        CTypedLayeredValue< CControl, CBrush > m_BorderBrush;
        CTypedLayeredValue< CControl, CFloatValue > m_BorderThickness;
        CTypedLayeredValue< CControl, CHorizontalAlignmentValue > m_HorizontalContentAlignment;
        CTypedLayeredValue< CControl, CVerticalAlignmentValue > m_VerticalContentAlignment;
        BOOL m_TemplateDirty;
        CUIElement* m_TemplateChild;
        CNamescope* m_TemplateNamescope;
};

template< >
struct ObjectTypeTraits< CControl >
{
    static const TypeIndex::Value Type = TypeIndex::Control;
};