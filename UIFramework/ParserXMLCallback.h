#pragma once

#include "XMLReader.h"

class CParserXMLCallback
{
    public:
        virtual __checkReturn HRESULT OnElementStart( 
			__in CXMLElementStart* pElementStart, 
			bool& Consumed 
			) = 0;

        virtual __checkReturn HRESULT OnElementEnd( 
			__in CXMLElementEnd* pElementEnd, 
			bool& Consumed
			) = 0;

        virtual __checkReturn HRESULT OnText(
			__in CXMLText* pText,
			bool& Consumed
			) = 0;

        virtual __checkReturn HRESULT OnAttribute(
			__in CXMLAttribute* pAttribute, 
			bool& Consumed 
			) = 0;
};