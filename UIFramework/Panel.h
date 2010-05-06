#pragma once

#include "FrameworkElement.h"

class CPanel : public CFrameworkElement
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

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );
};

template< >
struct ObjectTypeTraits< CPanel >
{
    static const TypeIndex::Value Type = TypeIndex::Panel;
};