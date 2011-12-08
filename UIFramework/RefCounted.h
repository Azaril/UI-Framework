#pragma once

#include "Types.h"

#ifdef FRAMEWORK_DEBUG

struct UIFRAMEWORK_API ITrackable
{
    virtual ~ITrackable(
        )
    {
    }

    virtual bool IgnoreTrackableForLeaks(
        )
    {
        return false;
    }
};


struct TrackableInformation
{
    UINT32 AllocationID;
    ITrackable* Object;
};

__out UIFRAMEWORK_API TrackableInformation* GetNewTrackableInformation(
    );

UIFRAMEWORK_API void FreeTrackableInformation(
    __in_opt TrackableInformation* pTrackableInformation
    );

struct ITrackableInformationCallback
{
    virtual void ProcessInformation(
        __in TrackableInformation* pInformation
        ) = 0;
};

UIFRAMEWORK_API void EnumerateTrackableInformation(
    __in ITrackableInformationCallback* pCallback
    );

template< typename T = Nothing >
class CTrackable : public T, 
                   public ITrackable
{
    protected:
        CTrackable(
            )
            : m_pTrackableInformation(NULL)
        {
            m_pTrackableInformation = GetNewTrackableInformation();

            m_pTrackableInformation->Object = this;
        }

        virtual ~CTrackable(
            )
        {
            FreeTrackableInformation(m_pTrackableInformation);
        }

    private:
        TrackableInformation* m_pTrackableInformation;
};

template< >
class CTrackable< Nothing > : public ITrackable
{
    protected:
        CTrackable(
            )
            : m_pTrackableInformation(NULL)
        {
            m_pTrackableInformation = GetNewTrackableInformation();

            m_pTrackableInformation->Object = this;
        }

        virtual ~CTrackable(
            )
        {
            FreeTrackableInformation(m_pTrackableInformation);
        }

    private:
        TrackableInformation* m_pTrackableInformation;
};

#define DISABLE_OBJECT_TRACKING() \
__override virtual bool IgnoreTrackableForLeaks(   \
    )   \
{   \
    return true;    \
}

#else

#define DISABLE_OBJECT_TRACKING()

#endif



template< typename T = Nothing >
class CRefCountedObjectBase : 
#ifdef FRAMEWORK_DEBUG
    public CTrackable< T >
#else
    public T
#endif
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
class CRefCountedObjectBase< Nothing > 
#ifdef FRAMEWORK_DEBUG
    : public CTrackable< Nothing >
#endif
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

template< typename T1, typename T2 >
inline void ReplaceObject(
    __inout T1*& pObj,
    __in_opt T2* pNewObj 
    )
{
    AddRefObject(pNewObj);
    ReleaseObject(pObj);

    pObj = pNewObj;
}

template< typename T1, typename T2 >
inline void SetObject(
    __inout T1*& pObj,
    __in_opt T2* pNewObj 
    )
{
    AddRefObject(pNewObj);

    pObj = pNewObj;
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

#define STATIC_REFCOUNTING()    \
__override virtual INT32 AddRef( \
    )  \
{   \
    return 1;  \
}   \
\
__override virtual INT32 Release( \
    ) \
{   \
    return 1; \
}