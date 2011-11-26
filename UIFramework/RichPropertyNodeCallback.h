#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "UIElement.h"
#include "XMLReader.h"
#include "ParseContext.h"
#include "PropertyNodeCallback.h"
#include "ElementNodeCallback.h"

class CRichPropertyNodeCallback : public CPropertyNodeCallback
{
    public:
        DECLARE_FACTORY3( CRichPropertyNodeCallback, CParseContext*, CPropertyInformation*, CXMLElementStart* );

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
        CRichPropertyNodeCallback(
			);

        virtual ~CRichPropertyNodeCallback(
			);

        __checkReturn HRESULT Initialize( 
			__in CParseContext* pContext, 
			__in CPropertyInformation* pProperties, 
			__in CXMLElementStart* pXMLStart 
			);

        bool m_Complete;
        bool m_SetTextValue;
        bool m_SetObjectValue;
        CElementNodeCallback* m_ChildNode;
        CProperty* m_Property;
        bool m_IsTemplate;
        CParserCommandList* m_ChildCommandList;
};