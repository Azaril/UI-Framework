#pragma once

#include "Types.h"

class CRefCountedObject
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
        CRefCountedObject() : m_Ref(1)
        {
        }
    
        virtual ~CRefCountedObject()
        {
        }
        
        INT32 m_Ref;
};

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