#pragma once

#include "Types.h"
#include "Collections.h"
#include "RefCounted.h"
#include "Factory.h"
#include "Providers.h"

struct IParserCallback
{
    virtual __checkReturn HRESULT OnPushObject( 
		__in CObjectWithType* pObject 
		) = 0;

    virtual __checkReturn HRESULT OnPopObject(
		__in CObjectWithType* pObject 
		) = 0;
};

class CParserCommandContext
{
    public:
        CParserCommandContext( 
			__in CProviders* pProviders,
			__in_opt IParserCallback* pCallback = NULL 
			);

        virtual ~CParserCommandContext(
			);

        __checkReturn HRESULT GetObject( 
			__deref_out CObjectWithType** ppObject 
			);

        __checkReturn HRESULT PushObject( 
			__in CObjectWithType* pObject 
			);

        __checkReturn HRESULT PopObject(
			);

        __out CProviders* GetProviders();

        template< typename T >
        __checkReturn HRESULT GetObject(
			__deref_out T** ppValue
			)
        {
            HRESULT hr = S_OK;
            CObjectWithType* pVal = NULL;

            IFC(GetObject(&pVal));

            IFCEXPECT(pVal->IsTypeOf(ObjectTypeTraits< T >::Type));

            *ppValue = (T*)pVal;
            pVal = NULL;

        Cleanup:
            ReleaseObject(pVal);

            return hr;
        }

    protected:
        CProviders* m_Providers;
        vector< CObjectWithType* > m_ObjectStack;
        IParserCallback* m_Callback;
};