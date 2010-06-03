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

        DECLARE_FACTORY( CCollection< T > );

        virtual HRESULT AddObject( CObjectWithType* pObject )
        {
            HRESULT hr = S_OK;
            T* pTypedObject = NULL;

            IFCPTR(pObject);

            IFCEXPECT(pObject->IsTypeOf(ObjectTypeTraits< T >::Type));

            pTypedObject = (T*)pObject;

            m_Collection.push_back(pTypedObject);
            AddRefObject(pTypedObject);

            for(typename std::vector< SubscriberType* >::iterator SubscriberIt = m_Subscribers.begin(); SubscriberIt != m_Subscribers.end(); ++SubscriberIt)
            {
                (*SubscriberIt)->OnItemAdded(pTypedObject);
            }

        Cleanup:
            return hr;
        }

        virtual HRESULT RemoveObject( CObjectWithType* pObject )
        {
            HRESULT hr = S_OK;

            IFCPTR(pObject);

            for(typename std::vector< T* >::iterator It = m_Collection.begin(); It != m_Collection.end(); ++It)
            {
                if((*It) == pObject)
                {
                    T* pTypedObject = *It;

                    m_Collection.erase(It);

                    for(typename std::vector< SubscriberType* >::iterator SubscriberIt = m_Subscribers.begin(); SubscriberIt != m_Subscribers.end(); ++SubscriberIt)
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

        HRESULT RemoveAtIndex(UINT32 Index)
        {
            HRESULT hr = S_OK;

            IFCEXPECT(Index < GetCount());

            m_Collection.erase(m_Collection.begin() + Index);

        Cleanup:
            return hr;
        }

        UINT32 GetCount()
        {
            return m_Collection.size();
        }

        T* GetAtIndex( UINT32 Index )
        {
            return m_Collection[Index];
        }

        HRESULT AddSubscriber(SubscriberType* pSubscriber)
        {
            HRESULT hr = S_OK;

            IFCPTR(pSubscriber);

            m_Subscribers.push_back(pSubscriber);

        Cleanup:
            return hr;
        }

        HRESULT RemoveSubscriber(SubscriberType* pSubscriber)
        {
            HRESULT hr = S_OK;

            IFCPTR(pSubscriber);

            for(typename std::vector< SubscriberType* >::iterator It = m_Subscribers.begin(); It != m_Subscribers.end(); ++It)
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
        CCollection()
        {
        }

        virtual ~CCollection()
        {
            for(typename std::vector< T* >::iterator It = m_Collection.begin(); It != m_Collection.end(); ++It)
            {
                (*It)->Release();
            }
        }

        HRESULT Initialize()
        {
            HRESULT hr = S_OK;

            return hr;
        }

        std::vector< T* > m_Collection;
        std::vector< SubscriberType* > m_Subscribers;
};
