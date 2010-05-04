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
        virtual BOOL IsComplete() = 0;

        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart, BOOL& Consumed );
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd, BOOL& Consumed );
        virtual HRESULT OnText( CXMLText* pText, BOOL& Consumed );
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute, BOOL& Consumed );

    protected:
        CParserNodeCallback();
        virtual ~CParserNodeCallback();

        HRESULT Initialize( CParseContext* pContext );

        CParseContext* m_Context;
};