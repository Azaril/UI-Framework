#pragma once

#include "RefCounted.h"

template< typename T = Nothing >
class CCOMRefCountedObjectBase : public T
{
    public:
        STDMETHOD_(ULONG, AddRef)(
            )
        {
            LONG refCount = InterlockedIncrement(&m_Ref);
            
            return refCount;
        }
        
        STDMETHOD_(ULONG, Release)(
            )
        {
            LONG refCount = InterlockedDecrement(&m_Ref);

            if(refCount == 0)
            {
                delete this;
            }
            
            return refCount;
        }

    protected:
        CCOMRefCountedObjectBase(
            ) 
            : m_Ref(1)
        {
        }
    
        virtual ~CCOMRefCountedObjectBase(
            )
        {
        }
        
        LONG m_Ref;
};

template< >
class CCOMRefCountedObjectBase< Nothing >
{
    public:
        STDMETHOD_(ULONG, AddRef)(
            )
        {
            LONG refCount = InterlockedIncrement(&m_Ref);
            
            return refCount;
        }
        
        STDMETHOD_(ULONG, Release)(
            )
        {
            LONG refCount = InterlockedDecrement(&m_Ref);

            if(refCount == 0)
            {
                delete this;
            }
            
            return refCount;
        }

    protected:
        CCOMRefCountedObjectBase(
            ) 
            : m_Ref(1)
        {
        }
    
        virtual ~CCOMRefCountedObjectBase(
            )
        {
        }
        
        LONG m_Ref;
};

typedef CCOMRefCountedObjectBase< > CCOMRefCountedObject;

#define DELEGATE_COM_REFCOUNTING( base ) \
STDMETHOD(ULONG, AddRef)( \
    )  \
{   \
    return base::AddRef();  \
}   \
\
STDMETHOD(ULONG, Release)( \
    ) \
{   \
    return base::Release(); \
}

#define DECLARE_COM_ADDREF_RELEASE_INTERFACE()   \
STDMETHOD(ULONG, AddRef)( \
    ) = 0;  \
\
STDMETHOD(ULONG, Release)( \
    ) = 0;
