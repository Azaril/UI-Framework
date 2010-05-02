#pragma once

#include "FrameworkElement.h"

class CPanel : public CFrameworkElement
{
    public:       
        virtual TypeIndex::Value GetType() { return TypeIndex::Panel; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Panel || CFrameworkElement::IsTypeOf(Type); }

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
