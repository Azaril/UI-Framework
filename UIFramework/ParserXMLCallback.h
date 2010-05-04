#pragma once

#include "XMLReader.h"

class CParserXMLCallback
{
    public:
        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart, BOOL& Consumed ) = 0;
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd, BOOL& Consumed ) = 0;
        virtual HRESULT OnText( CXMLText* pText, BOOL& Consumed ) = 0;
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute, BOOL& Consumed ) = 0;
};