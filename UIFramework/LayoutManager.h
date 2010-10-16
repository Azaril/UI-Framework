#pragma once

#include "Factory.h"
#include "UIElement.h"

class CLayoutManager : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CLayoutManager, CUIElement* );

        HRESULT SetLayoutSize( const SizeF& LayoutSize );
        HRESULT EnsureLayout();

    protected:
        CLayoutManager();
        virtual ~CLayoutManager();

        HRESULT Initialize( CUIElement* pRootElement );

        CUIElement* m_RootElement;
        SizeF m_LayoutSize;
};

