#include "Namescope.h"

CNamescope::CNamescope(
    )
{
}

CNamescope::~CNamescope(
    )
{
    for(std::vector< CNamedObjectHolder* >::iterator It = m_NamedObjects.begin(); It != m_NamedObjects.end(); ++It)
    {
        (*It)->Release();
    }

    m_NamedObjects.clear();
}

__checkReturn HRESULT 
CNamescope::Initialize(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CNamescope::InternalFindObject(
    __in_z const WCHAR* pName, 
    __deref_out_opt CNamedObjectHolder** ppHolder
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pName);
    IFCPTR(ppHolder);

    for(std::vector< CNamedObjectHolder* >::iterator It = m_NamedObjects.begin(); It != m_NamedObjects.end(); ++It)
    {
        CNamedObjectHolder* pHolder = (*It);

        if(wcscmp(pHolder->GetName(), pName) == 0)
        {
            *ppHolder = pHolder;
            AddRefObject(pHolder);

            goto Cleanup;
        }
    }

    *ppHolder = NULL;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CNamescope::RegisterName(
    __in_z const WCHAR* pName,
    __in CObjectWithType* pObject
    )
{
    HRESULT hr = S_OK;
    CNamedObjectHolder* pHolder = NULL;

    IFCPTR(pName);
    IFCPTR(pObject);

    IFC(InternalFindObject(pName, &pHolder));

    if(pHolder)
    {
        IFCEXPECT(pHolder->GetObject() == pObject);
    }
    else
    {
        IFC(CNamedObjectHolder::Create(pName, pObject, &pHolder));

        m_NamedObjects.push_back(pHolder);

        pHolder = NULL;
    }

Cleanup:
    ReleaseObject(pHolder);

    return hr;
}

__checkReturn HRESULT 
CNamescope::UnregisterName(
    __in_z const WCHAR* pName,
    __in CObjectWithType* pObject
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pName);
    IFCPTR(pObject);

    for(std::vector< CNamedObjectHolder* >::iterator It = m_NamedObjects.begin(); It != m_NamedObjects.end(); ++It)
    {
        CNamedObjectHolder* pHolder = (*It);

        if(wcscmp(pHolder->GetName(), pName) == 0)
        {
            IFCEXPECT(pHolder->GetObject() == pObject);

            ReleaseObject(pHolder);

            m_NamedObjects.erase(It);

            goto Cleanup;
        }
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CNamescope::FindName(
    __in_z const WCHAR* pName,
    __deref_out CObjectWithType** ppObject
    )
{
    HRESULT hr = S_OK;
    CNamedObjectHolder* pHolder = NULL;

    IFCPTR(pName);
    IFCPTR(ppObject);

    IFC(InternalFindObject(pName, &pHolder));

    if(pHolder)
    {
        CObjectWithType* pObject = pHolder->GetObject();

        *ppObject = pObject;
        AddRefObject(pObject);
    }
    else
    {
        *ppObject = NULL;
    }

Cleanup:
    ReleaseObject(pHolder);

    return hr;
}

CNamedObjectHolder::CNamedObjectHolder(
    ) 
    : m_Object(NULL)
{
}

CNamedObjectHolder::~CNamedObjectHolder()
{
    ReleaseObject(m_Object);
}

__checkReturn HRESULT 
CNamedObjectHolder::Initialize(
    __in_z const WCHAR* pName,
    __in CObjectWithType* pObject
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pName);
    IFCPTR(pObject);

    m_Name = pName;

    m_Object = pObject;
    AddRefObject(m_Object);

Cleanup:
    return hr;
}

__out_z const WCHAR* 
CNamedObjectHolder::GetName(
    )
{
    return m_Name.c_str();
}

__out CObjectWithType* 
CNamedObjectHolder::GetObject(
    )
{
    return m_Object;
}

//
// CNamescope
//
extern "C" __declspec(dllexport)
TypeIndex::Value
CNamescope_TypeIndex(
    )
{
    return TypeIndex::Namescope;
}

extern "C" __declspec(dllexport)
__out CObjectWithType* 
CNamescope_CastTo_CObjectWithType(
    __in CNamescope* pNamescope
    )
{
    return pNamescope;
}

extern "C" __declspec(dllexport)
__out_opt CNamescope* 
CObjectWithType_CastTo_CNamescope(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::Namescope)) ? (CNamescope*)pObject : NULL;
}

extern "C" __declspec(dllexport)
__checkReturn HRESULT
CNamescope_FindName(
    __in CNamescope* pScope, 
    __in_z const WCHAR* pName, 
    __deref_out_opt CObjectWithType** ppObject
    )
{
    return pScope->FindName(pName, ppObject);
}