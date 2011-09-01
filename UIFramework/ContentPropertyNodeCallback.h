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

        __override virtual BOOL IsComplete(
			);

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

        BOOL m_Complete;
        BOOL m_SetTextValue;
        BOOL m_SetObjectValue;
        CElementNodeCallback* m_ChildNode;
        CProperty* m_Property;
        CPropertyInformation* m_Properties;
        BOOL m_IsTemplate;
        CParserCommandList* m_ChildCommandList;
};