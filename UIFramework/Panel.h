#pragma once

#include "FrameworkElement.h"

class UIFRAMEWORK_API CPanel : public CFrameworkElement
{
    public:       
        DECLARE_TYPE_WITH_BASE( TypeIndex::Panel, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT AddChild( CUIElement* pElement );
        virtual HRESULT RemoveChild( CUIElement* pElement );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        static CStaticProperty ChildrenProperty;

    protected:
        CPanel();
        virtual ~CPanel();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        CTypedLocalLayeredValue< CPanel, CUIElementCollection > m_Children;
};

template< >
struct ObjectTypeTraits< CPanel >
{
    static const TypeIndex::Value Type = TypeIndex::Panel;
};