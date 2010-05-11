#pragma once

#include "Types.h"
#include "Collections.h"
#include "RefCounted.h"
#include "Factory.h"
#include "Providers.h"

class CParserCommandContext
{
    public:
        CParserCommandContext( CProviders* pProviders );
        virtual ~CParserCommandContext();

        HRESULT GetObject( CObjectWithType** ppObject );
        HRESULT PushObject( CObjectWithType* pObject );
        HRESULT PopObject();

        CProviders* GetProviders();

        template< typename T >
        HRESULT GetObject( T** ppValue )
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
        std::vector< CObjectWithType* > m_ObjectStack;
};