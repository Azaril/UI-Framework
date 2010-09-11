#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "ClassResolver.h"
#include "TypeConverter.h"
#include "Providers.h"

class UIFRAMEWORK_API CParser : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CParser, CProviders* );

        HRESULT LoadFromFile( const WCHAR* pPath, CObjectWithType** ppRootObject );
        HRESULT LoadFromString( const WCHAR* pMarkup, CObjectWithType** ppRootObject );

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

        template< typename T >
        HRESULT LoadFromString( const WCHAR* pMarkup, T** ppRootObject )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pRoot = NULL;

            IFC(LoadFromString(pMarkup, &pRoot));

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

        HRESULT Initialize( CProviders* pProviders );

        CProviders* m_Providers;
};
