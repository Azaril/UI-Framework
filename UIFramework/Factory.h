#pragma once

#include "Types.h"
#include "ErrorChecking.h"
#include "RefCounted.h"

#define DECLARE_FACTORY( ThisType ) \
static HRESULT Create( ThisType** ppOut ) \
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
static HRESULT Create( Arg1Type Arg1, ThisType** ppOut ) \
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
static HRESULT Create( Arg1Type Arg1, Arg2Type Arg2, ThisType** ppOut ) \
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
static HRESULT Create( Arg1Type Arg1, Arg2Type Arg2, Arg3Type Arg3, ThisType** ppOut ) \
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
static HRESULT Create( Arg1Type Arg1, Arg2Type Arg2, Arg3Type Arg3, Arg4Type Arg4, ThisType** ppOut ) \
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
