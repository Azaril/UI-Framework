#pragma once

#include "FrameworkElement.h"

class CPanel : public CFrameworkElement
{
    public:       
        virtual TypeIndex::Value GetType() { return TypeIndex::Panel; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Panel || CFrameworkElement::IsTypeOf(Type); }

        virtual HRESULT AddChild( CUIElement* pElement );
        virtual HRESULT RemoveChild( CUIElement* pElement );

    protected:
        CPanel();
        virtual ~CPanel();

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );
};
