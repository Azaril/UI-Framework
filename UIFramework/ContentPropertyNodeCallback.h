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
        DECLARE_FACTORY4( CContentPropertyNodeCallback, CParseContext*, CPropertyObject*, CPropertyInformation*, CXMLElementStart* );
        DECLARE_FACTORY4( CContentPropertyNodeCallback, CParseContext*, CPropertyObject*, CPropertyInformation*, CXMLText* );

        virtual BOOL IsComplete();

        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart, BOOL& Consumed );
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd, BOOL& Consumed );
        virtual HRESULT OnText( CXMLText* pText, BOOL& Consumed );
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute, BOOL& Consumed );

    protected:
        CContentPropertyNodeCallback();
        virtual ~CContentPropertyNodeCallback();

        HRESULT Initialize( CParseContext* pParseContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation );
        HRESULT Initialize( CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLElementStart* pXMLStart );
        HRESULT Initialize( CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLText* pXMLText );

        CPropertyObject* m_Parent;
        BOOL m_Complete;
        BOOL m_SetTextValue;
        BOOL m_SetObjectValue;
        CElementNodeCallback* m_ChildNode;
        CProperty* m_Property;
        CResolvedClass* m_ResolvedClass;
        CPropertyInformation* m_Properties;
};