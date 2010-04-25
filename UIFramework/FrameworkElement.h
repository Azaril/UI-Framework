#pragma once

#include "UIElement.h"

class CFrameworkElement : public CUIElement
{
    typedef std::vector< CUIElement* > ChildCollection;

    public:
        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

    protected:
        CFrameworkElement();
        virtual ~CFrameworkElement();

        virtual HRESULT AddLogicalChild( CUIElement* pElement );
        virtual HRESULT RemoveLogicalChild( CUIElement* pElement );

        ChildCollection m_Children;
};
