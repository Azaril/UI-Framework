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
        CRichPropertyNodeCallback(
			);

        virtual ~CRichPropertyNodeCallback(
			);

        __checkReturn HRESULT Initialize( 
			__in CParseContext* pContext, 
			__in CPropertyInformation* pProperties, 
			__in CXMLElementStart* pXMLStart 
			);

        BOOL m_Complete;
        BOOL m_SetTextValue;
        BOOL m_SetObjectValue;
        CElementNodeCallback* m_ChildNode;
        CProperty* m_Property;
        BOOL m_IsTemplate;
        CParserCommandList* m_ChildCommandList;
};