#pragma once

#include "FrameworkElement.h"

class CDecorator : public CFrameworkElement
{
    public:
        DECLARE_FACTORY( CDecorator );

        virtual HRESULT SetChild( CUIElement* pChild );
        CUIElement* GetChild();

    protected:
        CDecorator();
        virtual ~CDecorator();

        HRESULT Initialize();

        CUIElement* m_Child;
};
