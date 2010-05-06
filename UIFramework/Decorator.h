#pragma once

#include "FrameworkElement.h"

class CDecorator : public CFrameworkElement
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Decorator, CFrameworkElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetChild( CUIElement* pChild );
        CUIElement* GetChild();

        //
        // Properties
        //
        static CStaticProperty ChildProperty;

    protected:
        CDecorator();
        virtual ~CDecorator();

        HRESULT Initialize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );

        CUIElement* m_Child;
};

template< >
struct ObjectTypeTraits< CDecorator >
{
    static const TypeIndex::Value Type = TypeIndex::Decorator;
};