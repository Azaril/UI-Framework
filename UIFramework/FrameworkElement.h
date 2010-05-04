#pragma once

#include "UIElement.h"
#include "Collection.h"

class CUIElementCollection : public CCollection< CUIElement >
{
    public:
        DECLARE_FACTORY( CUIElementCollection );

        DECLARE_TYPE_WITH_BASE( TypeIndex::UIElementCollection, CCollection< CUIElement > );

        virtual HRESULT AddObject( CObjectWithType* pObject );
};

template< >
struct ObjectTypeTraits< CUIElementCollection >
{
    static const TypeIndex::Value Type = TypeIndex::UIElementCollection;
};

class CFrameworkElement : public CUIElement
{
    typedef std::vector< CUIElement* > ChildCollection;

    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::FrameworkElement, CUIElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        HRESULT SetName( const WCHAR* pName );

        virtual CNamescope* GetNamescope();

        HRESULT FindName( const WCHAR* pName, CObjectWithType** ppObject );

        template< typename T >
        HRESULT FindName( const WCHAR* pName, T** ppObject )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pFoundObject = NULL;

            IFC(FindName(pName, &pFoundObject));

            if(pFoundObject)
            {
                IFCEXPECT(pFoundObject->IsTypeOf(ObjectTypeTraits< T >::Type));

                *ppObject = (T*)pFoundObject;
                pFoundObject = NULL;
            }
            else
            {
                *ppObject = NULL;
            }

        Cleanup:
            ReleaseObject(pFoundObject);

            return hr;
        }

    protected:
        CFrameworkElement();
        virtual ~CFrameworkElement();

        HRESULT Initialize();

        virtual HRESULT AddLogicalChild( CUIElement* pElement );
        virtual HRESULT RemoveLogicalChild( CUIElement* pElement );

        virtual VOID OnChildAdded( CUIElement* pElement );
        virtual VOID OnChildRemoved( CUIElement* pElement );

        HRESULT InternalSetName( const WCHAR* pName );

        CUIElementCollection* m_Children;
        std::wstring m_Name;

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

template< >
struct ObjectTypeTraits< CFrameworkElement >
{
    static const TypeIndex::Value Type = TypeIndex::FrameworkElement;
};