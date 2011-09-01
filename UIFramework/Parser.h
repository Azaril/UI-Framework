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

        __checkReturn HRESULT LoadFromFile( 
			__in_z const WCHAR* pPath,
			__deref_out CObjectWithType** ppRootObject 
			);

        __checkReturn HRESULT LoadFromString( 
			__in_z const WCHAR* pMarkup, 
			__deref_out CObjectWithType** ppRootObject 
			);

        template< typename T >
        __checkReturn HRESULT LoadFromFile( 
			__in_z const WCHAR* pPath, 
			__deref_out T** ppRootObject 
			)
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
        __checkReturn HRESULT LoadFromString( 
			__in_z const WCHAR* pMarkup, 
			__deref_out T** ppRootObject 
			)
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
        CParser(
			);

        virtual ~CParser(
			);

        __checkReturn HRESULT Initialize( 
			__in CProviders* pProviders 
			);

        CProviders* m_Providers;
};
