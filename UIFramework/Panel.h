#pragma once

#include "FrameworkElement.h"

class CPanel : public CFrameworkElement
{
    public:       
        DECLARE_TYPE_WITH_BASE( TypeIndex::Panel, CFrameworkElement );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT AddChild( CUIElement* pElement );
        virtual HRESULT RemoveChild( CUIElement* pElement );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CPanel();
        virtual ~CPanel();

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );
};

template< >
struct ObjectTypeTraits< CPanel >
{
    static const TypeIndex::Value Type = TypeIndex::Panel;
};