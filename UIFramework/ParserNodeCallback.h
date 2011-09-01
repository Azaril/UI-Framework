#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "XMLReader.h"
#include "ParseContext.h"
#include "ParserXMLCallback.h"

class CParserNodeCallback : public CRefCountedObject,
                            public CParserXMLCallback
{
    public:
        virtual BOOL IsComplete(
			) = 0;

        __override virtual __checkReturn HRESULT OnElementStart( 
			__in CXMLElementStart* pElementStart,
			BOOL& Consumed 
			);

        __override virtual __checkReturn HRESULT OnElementEnd(
			__in CXMLElementEnd* pElementEnd, 
			BOOL& Consumed 
			);

        __override virtual __checkReturn HRESULT OnText(
			__in CXMLText* pText, 
			BOOL& Consumed 
			);

        __override virtual __checkReturn HRESULT OnAttribute(
			__in CXMLAttribute* pAttribute, 
			BOOL& Consumed 
			);

    protected:
        CParserNodeCallback(
			);

        virtual ~CParserNodeCallback(
			);

        __checkReturn HRESULT Initialize( 
			__in CParseContext* pContext 
			);

        CParseContext* m_Context;
};