#pragma once

#include "Types.h"

template< typename T = Nothing >
class CRefCountedObjectBase : public T
{
    public:
        INT32 AddRef()
        {
            ++m_Ref;
            
            return m_Ref;
        }
        
        INT32 Release()
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
class CRefCountedObjectBase< Nothing >
{
    public:
        INT32 AddRef()
        {
            ++m_Ref;
            
            return m_Ref;
        }
        
        INT32 Release()
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
void AddRefObject( T*& pObj )
{
    if(pObj)
    {
        pObj->AddRef();
    }
}

template< typename T >
void ReleaseObject( T*& pObj )
{
    if(pObj)
    {
        pObj->Release();
        pObj = NULL;
    }
}