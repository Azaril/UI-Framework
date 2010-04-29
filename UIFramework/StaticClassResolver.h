#pragma once

#include "ClassResolver.h"
#include "Factory.h"

class CStaticClassResolver : public CClassResolver
{
    public:
        DECLARE_FACTORY( CStaticClassResolver );

        virtual HRESULT ResolveType( const WCHAR* pTypeName, ClassType* pClassType );

    protected:
        CStaticClassResolver();
        virtual ~CStaticClassResolver();

        HRESULT Initialize();
};
