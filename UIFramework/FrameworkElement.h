#pragma once

#include "UIElement.h"

class CFrameworkElement : public CUIElement
{
    typedef std::vector< CUIElement* > ChildCollection;

    public:
        virtual TypeIndex::Value GetType() { return TypeIndex::FrameworkElement; }

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

    protected:
        CFrameworkElement();
        virtual ~CFrameworkElement();

        HRESULT Finalize();

        virtual HRESULT AddLogicalChild( CUIElement* pElement );
        virtual HRESULT RemoveLogicalChild( CUIElement* pElement );

        //virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        ChildCollection m_Children;
};
