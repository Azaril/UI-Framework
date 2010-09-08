#pragma once

#include "Types.h"

template< typename T = Nothing >
class UIFRAMEWORK_API CRefCountedObjectBase : public T
{
    public:
        virtual INT32 AddRef()
        {
            ++m_Ref;
            
            return m_Ref;
        }
        
        virtual INT32 Release()
        {
            INT32 Ref = --m_Ref;
            
            if(Ref == 0)
            {
                delete this;
            }
            
            return Ref;
        }
    protected:
        CRefCountedObjectBase() : m_Ref(1)
        {
        }
    
        virtual ~CRefCountedObjectBase()
        {
        }
        
        INT32 m_Ref;
};

template< >
class UIFRAMEWORK_API CRefCountedObjectBase< Nothing >
{
    public:
        virtual INT32 AddRef()
        {
            ++m_Ref;
            
            return m_Ref;
        }
        
        virtual INT32 Release()
        {
            INT32 Ref = --m_Ref;
            
            if(Ref == 0)
            {
                delete this;
            }
            
            return Ref;
        }

    protected:
        CRefCountedObjectBase() : m_Ref(1)
        {
        }
    
        virtual ~CRefCountedObjectBase()
        {
        }
        
        INT32 m_Ref;
};

typedef CRefCountedObjectBase< > CRefCountedObject;

template< typename T >
inline void AddRefObject( T*& pObj )
{
    if(pObj)
    {
        pObj->AddRef();
    }
}

template< typename T >
inline void ReleaseObject( T*& pObj )
{
    if(pObj)
    {
        pObj->Release();
        pObj = NULL;
    }
}

#define DELEGATE_REFCOUNTING( base )    \
virtual INT32 AddRef()  \
{   \
    return base::AddRef();  \
}   \
virtual INT32 Release() \
{   \
    return base::Release(); \
}
