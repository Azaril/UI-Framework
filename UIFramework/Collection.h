#pragma once

#include "PropertyObject.h"
#include "Collections.h"
#include "RefCounted.h"
#include "Factory.h"

template< typename T >
struct ICollectionSubscriber
{
    virtual void OnItemAdded( T* pItem ) = 0;
    virtual void OnItemRemoved( T* pItem ) = 0;
};

template< typename T >
class CCollection : public CRefCountedObjectBase< CObjectCollection >
{
    public:
        typedef ICollectionSubscriber< T > SubscriberType;

        typedef vector< T* > CollectionType;
        typedef vector< SubscriberType* > SubscriberCollectionType;

        DECLARE_FACTORY1( CCollection< T >, CProviders* );

        __override virtual __checkReturn HRESULT AddObject(
            __in CObjectWithType* pObject 
            )
        {
            HRESULT hr = S_OK;
            T* pTypedObject = NULL;

            IFCPTR(pObject);

            IFCEXPECT(pObject->IsTypeOf(ObjectTypeTraits< T >::Type));

            pTypedObject = (T*)pObject;

            m_Collection.push_back(pTypedObject);
            AddRefObject(pTypedObject);

            for(typename vector< SubscriberType* >::iterator SubscriberIt = m_Subscribers.begin(); SubscriberIt != m_Subscribers.end(); ++SubscriberIt)
            {
                (*SubscriberIt)->OnItemAdded(pTypedObject);
            }

        Cleanup:
            return hr;
        }

        __override virtual HRESULT RemoveObject(
            __in CObjectWithType* pObject 
            )
        {
            HRESULT hr = S_OK;

            IFCPTR(pObject);

            for(typename vector< T* >::iterator It = m_Collection.begin(); It != m_Collection.end(); ++It)
            {
                if((*It) == pObject)
                {
                    T* pTypedObject = *It;

                    m_Collection.erase(It);

                    for(typename vector< SubscriberType* >::iterator SubscriberIt = m_Subscribers.begin(); SubscriberIt != m_Subscribers.end(); ++SubscriberIt)
                    {
                        (*SubscriberIt)->OnItemRemoved(pTypedObject);
                    }

                    ReleaseObject(pObject);

                    goto Cleanup;
                }
            }

            IFC(E_FAIL);

        Cleanup:
            return hr;
        }

        __checkReturn HRESULT RemoveAtIndex(
            UINT32 Index
            )
        {
            HRESULT hr = S_OK;

            IFCEXPECT(Index < GetCount());

            m_Collection.erase(m_Collection.begin() + Index);

        Cleanup:
            return hr;
        }

        UINT32 GetCount(
            )
        {
            return m_Collection.size();
        }

        __out T* GetAtIndex(
            UINT32 Index 
            )
        {
            return m_Collection[Index];
        }

        __checkReturn HRESULT AddSubscriber(
            __in SubscriberType* pSubscriber
            )
        {
            HRESULT hr = S_OK;

            IFCPTR(pSubscriber);

            m_Subscribers.push_back(pSubscriber);

        Cleanup:
            return hr;
        }

        __checkReturn HRESULT RemoveSubscriber(
            __in SubscriberType* pSubscriber
            )
        {
            HRESULT hr = S_OK;

            IFCPTR(pSubscriber);

            for(typename vector< SubscriberType* >::iterator It = m_Subscribers.begin(); It != m_Subscribers.end(); ++It)
            {
                if((*It) == pSubscriber)
                {
                    m_Subscribers.erase(It);
                    goto Cleanup;
                }
            }

            IFC(E_FAIL);

        Cleanup:
            return hr;
        }

    protected:
        CCollection(
            )
        {
        }

        virtual ~CCollection(
            )
        {
            for(typename vector< T* >::iterator It = m_Collection.begin(); It != m_Collection.end(); ++It)
            {
                (*It)->Release();
            }
        }

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders
            )
        {
            HRESULT hr = S_OK;

            IFC(CObjectCollection::Initialize(pProviders));

        Cleanup:
            return hr;
        }

        CollectionType m_Collection;
        SubscriberCollectionType m_Subscribers;
};
