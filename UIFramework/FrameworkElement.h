#pragma once

#include "UIElement.h"
#include "Collection.h"
#include "ResourceDictionary.h"
#include "Style.h"
#include "LayeredValue.h"

class UIFRAMEWORK_API CUIElementCollection : public CCollection< CUIElement >
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

class UIFRAMEWORK_API CFrameworkElement : public CUIElement
{
    typedef vector< CUIElement* > ChildCollection;

    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::FrameworkElement, CUIElement );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        virtual HRESULT Measure( SizeF Size );

        virtual HRESULT Arrange( RectF Bounds );

        void SetAutomaticNamescopeParticipation( BOOL Participate );

        HRESULT RegisterInNamescope( CNamescope* pNamescope, CStringValue** ppRegisteredName );

        HRESULT SetName( const WCHAR* pName );

        HRESULT FindName( const WCHAR* pName, CObjectWithType** ppObject );

        virtual HRESULT EnsureLoaded();

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

            IFC_NOTRACE(FindResource(pKey, &pFoundObject));

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

        __override virtual __out_opt const CFontDescription* GetEffectiveFontDescription(
            );

        //
        // Properties
        //
        static CStaticProperty NameProperty;
        static CStaticProperty ResourcesProperty;
        static CStaticProperty StyleProperty;

    protected:
        CFrameworkElement();
        virtual ~CFrameworkElement();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual HRESULT AddLogicalChild( CUIElement* pElement );
        virtual HRESULT RemoveLogicalChild( CUIElement* pElement );

        virtual void OnChildAdded( CUIElement* pElement );
        virtual void OnChildRemoved( CUIElement* pElement );

        HRESULT EnsureStyle();
        HRESULT RevokeStyle();
        virtual HRESULT ApplyStyle( CStyle* pStyle );
        virtual HRESULT SetStyleValue( CProperty* pProperty, CObjectWithType* pValue );

        virtual CUIElement* GetTemplateParentForChildren();
        virtual CNamescope* GetNamescopeForChildren();

        static HRESULT SetStyleBindingValue( CPropertyObject* pTarget, CProperty* pTargetProperty, CObjectWithType* pValue );

        HRESULT SetStyleBinding( CProperty* pProperty, CBindingBase* pBinding );
        HRESULT SetStyleValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnNameChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnStyleChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnResourcesChanged );

        HRESULT OnNameChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnStyleChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnResourcesChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetEffectiveName( CStringValue** ppName );
        HRESULT GetEffectiveStyle( CStyle** ppStyle );
        HRESULT GetEffectiveResources( CResourceDictionary** ppResources );

        CUIElementCollection* GetChildCollection();

        CTypedLocalLayeredValue< CFrameworkElement, CStringValue > m_Name;
        CTypedLocalLayeredValue< CFrameworkElement, CResourceDictionary > m_Resources;
        CTypedLocalLayeredValue< CFrameworkElement, CStyle > m_Style;

        BOOL m_AutomaticNamescopeParticipation;
        CStringValue* m_RegisteredName;

        BOOL m_StyleDirty;
        CResolvedStyle* m_ResolvedStyle;

        BOOL m_ChildrenNeedLoading;

    private:
        CUIElementCollection* m_Children;

        class UIFRAMEWORK_API CChildrenSubscriber : public CUIElementCollection::SubscriberType
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

        class UIFRAMEWORK_API CStyleCallback : public IStyleCallback
        {
            public:
                CStyleCallback( CFrameworkElement& This ) : m_This(This)
                {
                }

                virtual HRESULT SetStyleValue( CProperty* pProperty, CObjectWithType* pValue )
                {
                    return m_This.SetStyleValue(pProperty, pValue);
                }

            protected:
                CFrameworkElement& m_This;

        } m_StyleCallback;
};

template< >
struct ObjectTypeTraits< CFrameworkElement >
{
    static const TypeIndex::Value Type = TypeIndex::FrameworkElement;
};