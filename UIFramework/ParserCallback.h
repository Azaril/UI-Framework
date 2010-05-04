#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "UIElement.h"
#include "XMLReader.h"
#include "ParseContext.h"
#include "ElementNodeCallback.h"

class CParserCallback : public CRefCountedObject,
                        public CXMLReaderCallback                        
{
    public:
        DECLARE_FACTORY2( CParserCallback, CClassResolver*, CTypeConverter* );

        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart );
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd );
        virtual HRESULT OnText( CXMLText* pText );
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute );

        HRESULT GetRootObject( CObjectWithType** ppRootObject );

        template< typename T >
        HRESULT GetRootObject( T** ppRootObject )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pRoot = NULL;

            IFC(GetRootObject(&pRoot));

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
        CParserCallback();
        virtual ~CParserCallback();

        HRESULT Initialize( CClassResolver* pClassResolver, CTypeConverter* );

        CElementNodeCallback* m_ChildNode;
        CParseContext* m_ParseContext;
};