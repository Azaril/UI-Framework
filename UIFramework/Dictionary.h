#pragma once

#include "PropertyObject.h"
#include "Collections.h"
#include "RefCounted.h"
#include "Factory.h"

template< typename T >
class CDictionary : public CRefCountedObjectBase< CObjectDictionary >
{
    class CDictionaryEntry
    {
        public:
            CDictionaryEntry( const CDictionaryEntry& Other ) : m_Key(Other.m_Key),
                                                                m_Value(Other.m_Value)
            {
                AddRefObject(m_Key);
                AddRefObject(m_Value);
            }

            CDictionaryEntry( CObjectWithType* pKey, T* pObject ) : m_Key(pKey),
                                                                    m_Value(pObject)
            {
                AddRefObject(m_Key);
                AddRefObject(m_Value);
            }

            ~CDictionaryEntry()
            {
                ReleaseObject(m_Key);
                ReleaseObject(m_Value);
            }

            CObjectWithType* GetKey()
            {
                return m_Key;
            }

            T* GetValue()
            {
                return m_Value;
            }

            HRESULT SetValue( T* pObject )
            {
                HRESULT hr = S_OK;

                IFCPTR(pObject);

                ReleaseObject(m_Value);

                m_Value = pObject;

                AddRefObject(m_Value);

            Cleanup:
                return hr;
            }

        protected:
            CObjectWithType* m_Key;
            T* m_Value;
    };

    public:
        DECLARE_FACTORY( CDictionary< T > );

        virtual HRESULT AddObject( CObjectWithType* pKey, CObjectWithType* pObject )
        {
            HRESULT hr = S_OK;
            T* pTypedObject = NULL;

            IFCPTR(pKey);
            IFCPTR(pObject);

            IFCEXPECT(pObject->IsTypeOf(ObjectTypeTraits< T >::Type));

            pTypedObject = (T*)pObject;

            for(typename std::vector< CDictionaryEntry >::iterator It = m_Collection.begin(); It != m_Collection.end(); ++It)
            {
                CObjectWithType* pOtherKey = It->GetKey();

                if(pOtherKey->Equals(pKey))
                {
                    IFC(It->SetValue(pObject));

                    goto Cleanup;
                }
            }

            m_Collection.push_back(CDictionaryEntry(pKey, pTypedObject));

        Cleanup:
            return hr;
        }

        virtual HRESULT RemoveObject( CObjectWithType* pKey, CObjectWithType* pObject )
        {
            HRESULT hr = S_OK;

            IFCPTR(pKey);
            IFCPTR(pObject);

            for(typename std::vector< CDictionaryEntry >::iterator It = m_Collection.begin(); It != m_Collection.end(); ++It)
            {
                CObjectWithType* pOtherKey = It->GetKey();

                if(pOtherKey->Equals(pKey) && pObject == It->GetValue())
                {
                    m_Collection.erase(It);

                    goto Cleanup;
                }
            }

            IFC(E_FAIL);

        Cleanup:
            return hr;
        }

        virtual HRESULT GetObject( CObjectWithType* pKey, CObjectWithType** ppObject )
        {
            HRESULT hr = S_OK;

            IFCPTR(pKey);
            IFCPTR(ppObject);

            for(typename std::vector< CDictionaryEntry >::iterator It = m_Collection.begin(); It != m_Collection.end(); ++It)
            {
                CObjectWithType* pOtherKey = It->GetKey();

                if(pOtherKey->Equals(pKey))
                {
                    T* pTypedObject = It->GetValue();

                    *ppObject = pTypedObject;
                    AddRefObject(pTypedObject);

                    goto Cleanup;
                }
            }

            hr = E_FAIL;

        Cleanup:
            return hr;
        }

    protected:
        CDictionary()
        {
        }

        virtual ~CDictionary()
        {
        }

        HRESULT Initialize()
        {
            HRESULT hr = S_OK;

            return hr;
        }

        std::vector< CDictionaryEntry > m_Collection;
};
