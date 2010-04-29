#pragma once

#include "RefCounted.h"
#include "PropertyObject.h"

class CUIElement;

typedef HRESULT (*CreateTypeFunc)( CPropertyObject** ppObject );

struct ClassType
{
    CreateTypeFunc CreateType;
};

class CClassResolver : public CRefCountedObject
{
    public:
        virtual HRESULT ResolveType( const WCHAR* pTypeName, ClassType* pClassType ) = 0;
};