#pragma once

#include "FrameworkElement.h"

class CDecorator : public CFrameworkElement
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Decorator, CFrameworkElement );

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

template< >
struct ObjectTypeTraits< CDecorator >
{
    static const TypeIndex::Value Type = TypeIndex::Decorator;
};