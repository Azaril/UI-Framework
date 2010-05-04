#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "ClassResolver.h"
#include "TypeConverter.h"

class CParser : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CParser, CClassResolver*, CTypeConverter* );

        HRESULT LoadFromFile( const WCHAR* pPath, CObjectWithType** ppRootObject );

        template< typename T >
        HRESULT LoadFromFile( const WCHAR* pPath, T** ppRootObject )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pRoot = NULL;

            IFC(LoadFromFile(pPath, &pRoot));

            if(pRoot)
            {
                IFCEXPECT(pRoot->IsTypeOf(ObjectTypeTraits< T >::Type));

                *ppRootObject = (T*)pRoot;
                pRoot = NULL;
            }
            else
            {
                *ppRootObject = NULL;
            }

        Cleanup:
            ReleaseObject(pRoot);

            return hr;
        }

    protected:
        CParser();
        virtual ~CParser();

        HRESULT Initialize( CClassResolver* pResolver, CTypeConverter* pTypeConverter );

        CClassResolver* m_ClassResolver;
        CTypeConverter* m_TypeConverter;
};
