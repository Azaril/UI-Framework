#pragma once

#include "Types.h"
#include "ErrorChecking.h"
#include "RefCounted.h"

#define DECLARE_FACTORY( ThisType ) \
static __checkReturn HRESULT Create( \
	__deref_out ThisType** ppOut \
	) \
{ \
    HRESULT hr = S_OK; \
    ThisType* pObj = NULL; \
    IFCPTR(ppOut); \
    pObj = new ThisType(); \
    IFCOOM(pObj); \
    IFC(pObj->Initialize()); \
    *ppOut = pObj; \
    pObj = NULL; \
Cleanup: \
    ReleaseObject(pObj); \
    return hr; \
}

#define DECLARE_FACTORY1( ThisType, Arg1Type ) \
static __checkReturn HRESULT Create( \
	Arg1Type Arg1, \
	__deref_out ThisType** ppOut \
	) \
{ \
    HRESULT hr = S_OK; \
    ThisType* pObj = NULL; \
    IFCPTR(ppOut); \
    pObj = new ThisType(); \
    IFCOOM(pObj); \
    IFC(pObj->Initialize(Arg1)); \
    *ppOut = pObj; \
    pObj = NULL; \
Cleanup: \
    ReleaseObject(pObj); \
    return hr; \
}

#define DECLARE_FACTORY2( ThisType, Arg1Type, Arg2Type ) \
static __checkReturn HRESULT Create( \
	Arg1Type Arg1, \
	Arg2Type Arg2, \
	__deref_out ThisType** ppOut \
	) \
{ \
    HRESULT hr = S_OK; \
    ThisType* pObj = NULL; \
    IFCPTR(ppOut); \
    pObj = new ThisType(); \
    IFCOOM(pObj); \
    IFC(pObj->Initialize(Arg1, Arg2)); \
    *ppOut = pObj; \
    pObj = NULL; \
Cleanup: \
    ReleaseObject(pObj); \
    return hr; \
}

#define DECLARE_FACTORY3( ThisType, Arg1Type, Arg2Type, Arg3Type ) \
static __checkReturn HRESULT Create( \
	Arg1Type Arg1, \
	Arg2Type Arg2, \
	Arg3Type Arg3, \
	__deref_out ThisType** ppOut \
	) \
{ \
    HRESULT hr = S_OK; \
    ThisType* pObj = NULL; \
    IFCPTR(ppOut); \
    pObj = new ThisType(); \
    IFCOOM(pObj); \
    IFC(pObj->Initialize(Arg1, Arg2, Arg3)); \
    *ppOut = pObj; \
    pObj = NULL; \
Cleanup: \
    ReleaseObject(pObj); \
    return hr; \
}

#define DECLARE_FACTORY4( ThisType, Arg1Type, Arg2Type, Arg3Type, Arg4Type ) \
static __checkReturn HRESULT Create( \
	Arg1Type Arg1, \
	Arg2Type Arg2, \
	Arg3Type Arg3, \
	Arg4Type Arg4, \
	__deref_out ThisType** ppOut \
	) \
{ \
    HRESULT hr = S_OK; \
    ThisType* pObj = NULL; \
    IFCPTR(ppOut); \
    pObj = new ThisType(); \
    IFCOOM(pObj); \
    IFC(pObj->Initialize(Arg1, Arg2, Arg3, Arg4)); \
    *ppOut = pObj; \
    pObj = NULL; \
Cleanup: \
    ReleaseObject(pObj); \
    return hr; \
}

#define DECLARE_FACTORY5( ThisType, Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type ) \
static __checkReturn HRESULT Create( \
	Arg1Type Arg1, \
	Arg2Type Arg2, \
	Arg3Type Arg3, \
	Arg4Type Arg4, \
	Arg5Type Arg5, \
	__deref_out ThisType** ppOut \
	) \
{ \
    HRESULT hr = S_OK; \
    ThisType* pObj = NULL; \
    IFCPTR(ppOut); \
    pObj = new ThisType(); \
    IFCOOM(pObj); \
    IFC(pObj->Initialize(Arg1, Arg2, Arg3, Arg4, Arg5)); \
    *ppOut = pObj; \
    pObj = NULL; \
Cleanup: \
    ReleaseObject(pObj); \
    return hr; \
}

#define DECLARE_FACTORY6( ThisType, Arg1Type, Arg2Type, Arg3Type, Arg4Type, Arg5Type, Arg6Type ) \
static __checkReturn HRESULT Create( \
	Arg1Type Arg1, \
	Arg2Type Arg2, \
	Arg3Type Arg3, \
	Arg4Type Arg4, \
	Arg5Type Arg5, \
	Arg6Type Arg6, \
	__deref_out ThisType** ppOut \
	) \
{ \
    HRESULT hr = S_OK; \
    ThisType* pObj = NULL; \
    IFCPTR(ppOut); \
    pObj = new ThisType(); \
    IFCOOM(pObj); \
    IFC(pObj->Initialize(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)); \
    *ppOut = pObj; \
    pObj = NULL; \
Cleanup: \
    ReleaseObject(pObj); \
    return hr; \
}
