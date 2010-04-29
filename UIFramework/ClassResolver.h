#pragma once

#include "RefCounted.h"

class CUIElement;

typedef HRESULT (*CreateTypeFunc)( CUIElement** ppElement );

struct ClassType
{
    CreateTypeFunc CreateType;
};

class CClassResolver : public CRefCountedObject
{
    public:
        virtual HRESULT ResolveType( const WCHAR* pTypeName, ClassType* pClassType ) = 0;
};