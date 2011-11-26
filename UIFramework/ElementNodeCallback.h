#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "UIElement.h"
#include "XMLReader.h"
#include "ParseContext.h"
#include "ParserNodeCallback.h"
#include "PropertyNodeCallback.h"

class CElementNodeCallback : public CParserNodeCallback
{
    public:
        DECLARE_FACTORY2( CElementNodeCallback, CParseContext*, CXMLElementStart* );

        CStringValue* GetKey(
			);

        __override virtual bool IsComplete(
			);

        __override virtual __checkReturn HRESULT OnElementStart( 
			__in CXMLElementStart* pElementStart, 
			bool& Consumed 
			);

        __override virtual __checkReturn HRESULT OnElementEnd( 
			__in CXMLElementEnd* pElementEnd, 
			bool& Consumed 
			);

        __override virtual __checkReturn HRESULT OnText( 
			__in CXMLText* pText, 
			bool& Consumed 
			);

        __override virtual __checkReturn HRESULT OnAttribute( 
			__in CXMLAttribute* pAttribute, 
			bool& Consumed 
			);

    protected:
        CElementNodeCallback(
			);

        virtual ~CElementNodeCallback(
			);

        __checkReturn HRESULT Initialize(
			__in CParseContext* pContext, 
			__in CXMLElementStart* pXMLStart 
			);

        CPropertyNodeCallback* m_ChildNode;
        CResolvedClass* m_ResolvedClass;
        CPropertyInformation* m_Properties;
        bool m_Complete;
        CStringValue* m_KeyString;
};