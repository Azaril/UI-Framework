#pragma once

#include "UIElement.h"
#include "Collection.h"

class CUIElementCollection : public CCollection< CUIElement >
{
    public:
        DECLARE_FACTORY( CUIElementCollection );

        virtual TypeIndex::Value GetType() { return TypeIndex::UIElementCollection; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::UIElementCollection || CCollection< CUIElement >::IsTypeOf(Type); }

        virtual HRESULT AddObject( CObjectWithType* pObject );
};

class CFrameworkElement : public CUIElement
{
    typedef std::vector< CUIElement* > ChildCollection;

    public:
        virtual TypeIndex::Value GetType() { return TypeIndex::FrameworkElement; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::FrameworkElement || CUIElement::IsTypeOf(Type); }

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

    protected:
        CFrameworkElement();
        virtual ~CFrameworkElement();

        HRESULT Initialize();

        virtual HRESULT AddLogicalChild( CUIElement* pElement );
        virtual HRESULT RemoveLogicalChild( CUIElement* pElement );

        //virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual VOID OnChildAdded( CUIElement* pElement );
        virtual VOID OnChildRemoved( CUIElement* pElement );

        CUIElementCollection* m_Children;

        class CChildrenSubscriber : public CUIElementCollection::SubscriberType
        {
            public:
                CChildrenSubscriber(CFrameworkElement& This) : m_This(This)
                {
                }

                virtual void OnItemAdded(CUIElement* pItem)
                {
                    m_This.OnChildAdded(pItem);
                }

                virtual void OnItemRemoved(CUIElement* pItem)
                {
                    m_This.OnChildRemoved(pItem);
                }

            CFrameworkElement& m_This;
        } m_ChildrenSubscriber;
};
