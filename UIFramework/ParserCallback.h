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

        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart );
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd );
        virtual HRESULT OnText( CXMLText* pText );
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute );

    protected:
        CParserCallback();
        virtual ~CParserCallback();

        HRESULT Initialize( CParseContext* pContext );

        CElementNodeCallback* m_ChildNode;
        CParseContext* m_ParseContext;
};