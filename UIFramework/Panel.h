#pragma once

#include "FrameworkElement.h"

class CPanel : public CFrameworkElement
{
    public:       
        virtual HRESULT AddChild( CUIElement* pElement );
        virtual HRESULT RemoveChild( CUIElement* pElement );

    protected:
        CPanel();
        virtual ~CPanel();

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );
};
