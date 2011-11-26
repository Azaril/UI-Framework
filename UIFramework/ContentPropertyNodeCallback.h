#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "UIElement.h"
#include "XMLReader.h"
#include "ParseContext.h"
#include "ParserNodeCallback.h"
#include "PropertyNodeCallback.h"
#include "ElementNodeCallback.h"

class CContentPropertyNodeCallback : public CPropertyNodeCallback
{
    public:
        DECLARE_FACTORY3( CContentPropertyNodeCallback, CParseContext*, CPropertyInformation*, CXMLElementStart* );
        DECLARE_FACTORY3( CContentPropertyNodeCallback, CParseContext*, CPropertyInformation*, CXMLText* );

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
        CContentPropertyNodeCallback(
			);

        virtual ~CContentPropertyNodeCallback(
			);

        __checkReturn HRESULT Initialize( 
			__in CParseContext* pParseContext, 
			__in CPropertyInformation* pPropertyInformation 
			);

        __checkReturn HRESULT Initialize(
			__in CParseContext* pContext, 
			__in CPropertyInformation* pPropertyInformation, 
			__in CXMLElementStart* pXMLStart 
			);

        __checkReturn HRESULT Initialize(
			__in CParseContext* pContext, 
			__in CPropertyInformation* pPropertyInformation, 
			__in CXMLText* pXMLText 
			);

        bool m_Complete;
        bool m_SetTextValue;
        bool m_SetObjectValue;
        CElementNodeCallback* m_ChildNode;
        CProperty* m_Property;
        CPropertyInformation* m_Properties;
        bool m_IsTemplate;
        CParserCommandList* m_ChildCommandList;
};