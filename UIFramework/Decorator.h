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

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnChildChanged );

        HRESULT OnChildChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetEffectiveChild( CUIElement** ppChild );

        CTypedLayeredValue< CUIElement > m_Child;
};

template< >
struct ObjectTypeTraits< CDecorator >
{
    static const TypeIndex::Value Type = TypeIndex::Decorator;
};