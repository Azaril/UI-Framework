#pragma once

#include "FrameworkElement.h"

class CDecorator : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CDecorator );

        virtual TypeIndex::Value GetType() { return TypeIndex::Decorator; }

        virtual HRESULT SetChild( CUIElement* pChild );
        CUIElement* GetChild();

    protected:
        CDecorator();
        virtual ~CDecorator();

        HRESULT Initialize();

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        CUIElement* m_Child;
};
