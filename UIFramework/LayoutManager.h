#pragma once

#include "Factory.h"
#include "UIElement.h"

class CLayoutManager : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CLayoutManager, CUIElement* );

        __checkReturn HRESULT SetLayoutSize( 
            const SizeF& LayoutSize 
            );

        __checkReturn HRESULT EnsureLayout(
            );

    protected:
        CLayoutManager(
            );

        virtual ~CLayoutManager(
            );

        __checkReturn HRESULT Initialize( 
            __in CUIElement* pRootElement 
            );

        CUIElement* m_RootElement;
        SizeF m_LayoutSize;
};

