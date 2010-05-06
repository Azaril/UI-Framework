#pragma once

#include "UIElement.h"
#include "Collection.h"
#include "ResourceDictionary.h"
#include "Style.h"
#include "LayeredValue.h"

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

        HRESULT FindResource( CObjectWithType* pKey, CObjectWithType** ppObject );
        HRESULT FindResource( const WCHAR* pResourceName, CObjectWithType** ppObject );

        template< typename T >
        HRESULT FindResource( CObjectWithType* pKey, T** ppObject )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pFoundObject = NULL;

            IFC(FindResource(pKey, &pFoundObject));

            IFCEXPECT(pFoundObject->IsTypeOf(ObjectTypeTraits< T >::Type));

            *ppObject = (T*)pFoundObject;
            pFoundObject = NULL;

        Cleanup:
            ReleaseObject(pFoundObject);

            return hr;
        }

        template< typename T >
        HRESULT FindResource( const WCHAR* pResourceName, T** ppObject )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pFoundObject = NULL;

            IFC(FindResource(pResourceName, &pFoundObject));

            IFCEXPECT(pFoundObject->IsTypeOf(ObjectTypeTraits< T >::Type));

            *ppObject = (T*)pFoundObject;
            pFoundObject = NULL;

        Cleanup:
            ReleaseObject(pFoundObject);

            return hr;
        }

        //
        // Properties
        //
        static CStaticProperty NameProperty;
        static CStaticProperty ResourcesProperty;
        static CStaticProperty StyleProperty;

    protected:
        CFrameworkElement();
        virtual ~CFrameworkElement();

        HRESULT Initialize();

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT AddLogicalChild( CUIElement* pElement );
        virtual HRESULT RemoveLogicalChild( CUIElement* pElement );

        virtual VOID OnChildAdded( CUIElement* pElement );
        virtual VOID OnChildRemoved( CUIElement* pElement );

        HRESULT SetNameInternal( const WCHAR* pName );
        HRESULT SetStyleInternal( CStyle* pStyle );

        HRESULT EnsureStyle();
        virtual HRESULT ApplyStyle( CStyle* pStyle );
        virtual HRESULT OnStyleSetterResolved( CProperty* pProperty, CObjectWithType* pValue );

        CUIElementCollection* m_Children;
        std::wstring m_Name;
        CResourceDictionary* m_Resources;
        CStyle* m_Style;
        BOOL m_StyleDirty;

        class CChildrenSubscriber : public CUIElementCollection::SubscriberType
        {
            public:
                CChildrenSubscriber( CFrameworkElement& This ) : m_This(This)
                {
                }

                virtual void OnItemAdded( CUIElement* pItem )
                {
                    m_This.OnChildAdded(pItem);
                }

                virtual void OnItemRemoved( CUIElement* pItem )
                {
                    m_This.OnChildRemoved(pItem);
                }

            protected:
                CFrameworkElement& m_This;

        } m_ChildrenSubscriber;

        class CSetterResolverCallback : public IResolvedStyleSetterCallback
        {
            public:
                CSetterResolverCallback( CFrameworkElement& This ) : m_This(This)
                {
                }

                virtual HRESULT OnResolvedSetter( CProperty* pProperty, CObjectWithType* pValue )
                {
                    return m_This.OnStyleSetterResolved(pProperty, pValue);
                }

            protected:
                CFrameworkElement& m_This;

        } m_SetterResolvedCallback;
};

template< >
struct ObjectTypeTraits< CFrameworkElement >
{
    static const TypeIndex::Value Type = TypeIndex::FrameworkElement;
};