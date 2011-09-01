#pragma once

#include "XMLReader.h"

class CParserXMLCallback
{
    public:
        virtual __checkReturn HRESULT OnElementStart( 
			__in CXMLElementStart* pElementStart, 
			BOOL& Consumed 
			) = 0;

        virtual __checkReturn HRESULT OnElementEnd( 
			__in CXMLElementEnd* pElementEnd, 
			BOOL& Consumed
			) = 0;

        virtual __checkReturn HRESULT OnText(
			__in CXMLText* pText,
			BOOL& Consumed
			) = 0;

        virtual __checkReturn HRESULT OnAttribute(
			__in CXMLAttribute* pAttribute, 
			BOOL& Consumed 
			) = 0;
};