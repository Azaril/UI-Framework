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
        DECLARE_FACTORY1( CParserCallback, CParseContext* );

        __override virtual __checkReturn HRESULT OnElementStart( 
			__in CXMLElementStart* pElementStart 
			);

        __override virtual __checkReturn HRESULT OnElementEnd(
			__in CXMLElementEnd* pElementEnd 
			);

        __override virtual __checkReturn HRESULT OnText(
			__in CXMLText* pText 
			);

        __override virtual __checkReturn HRESULT OnAttribute(
			__in CXMLAttribute* pAttribute 
			);

    protected:
        CParserCallback(
			);

        virtual ~CParserCallback(
			);

        __checkReturn HRESULT Initialize( 
			__in CParseContext* pContext 
			);

        CElementNodeCallback* m_ChildNode;
        CParseContext* m_ParseContext;
};