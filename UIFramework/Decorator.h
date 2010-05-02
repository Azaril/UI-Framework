#pragma once

#include "FrameworkElement.h"

class CDecorator : public CFrameworkElement
{
    public:
        virtual TypeIndex::Value GetType() { return TypeIndex::Decorator; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Decorator || CFrameworkElement::IsTypeOf(Type); }

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT SetChild( CUIElement* pChild );
        CUIElement* GetChild();

    protected:
        CDecorator();
        virtual ~CDecorator();

        HRESULT Initialize();

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        CUIElement* m_Child;
};
