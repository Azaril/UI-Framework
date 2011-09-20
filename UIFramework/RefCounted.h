#pragma once

#include "Types.h"

template< typename T = Nothing >
class UIFRAMEWORK_API CRefCountedObjectBase : public T
{
    public:
        virtual INT32 AddRef(
            )
        {
            ++m_Ref;
            
            return m_Ref;
        }
        
        virtual INT32 Release(
            )
        {
            INT32 Ref = --m_Ref;
            
            if(Ref == 0)
            {
                delete this;
            }
            
            return Ref;
        }

    protected:
        CRefCountedObjectBase(
            ) 
            : m_Ref(1)
        {
        }
    
        virtual ~CRefCountedObjectBase(
            )
        {
        }
        
        INT32 m_Ref;
};

template< >
class UIFRAMEWORK_API CRefCountedObjectBase< Nothing >
{
    public:
        virtual INT32 AddRef(
            )
        {
            ++m_Ref;
            
            return m_Ref;
        }
        
        virtual INT32 Release(
            )
        {
            INT32 Ref = --m_Ref;
            
            if(Ref == 0)
            {
                delete this;
            }
            
            return Ref;
        }

    protected:
        CRefCountedObjectBase(
            ) 
            : m_Ref(1)
        {
        }
    
        virtual ~CRefCountedObjectBase(
            )
        {
        }
        
        INT32 m_Ref;
};

typedef CRefCountedObjectBase< > CRefCountedObject;

template< typename T >
inline void AddRefObject( 
    __in_opt T*& pObj 
    )
{
    if(pObj)
    {
        pObj->AddRef();
    }
}

template< typename T >
inline void ReleaseObject(
    __in_opt T*& pObj 
    )
{
    if(pObj)
    {
        pObj->Release();
        pObj = NULL;
    }
}

#define DELEGATE_REFCOUNTING( base )    \
__override virtual INT32 AddRef( \
    )  \
{   \
    return base::AddRef();  \
}   \
\
__override virtual INT32 Release( \
    ) \
{   \
    return base::Release(); \
}

#define DECLARE_ADDREF_RELEASE_INTERFACE()   \
virtual INT32 AddRef(   \
    ) = 0;  \
\
virtual INT32 Release(  \
    ) = 0;
