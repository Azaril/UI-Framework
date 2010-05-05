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
        DECLARE_FACTORY3( CElementNodeCallback, CParseContext*, CPropertyObject*, CXMLElementStart* );

        CPropertyObject* GetObject();
        CObjectWithType* GetKey();

        virtual BOOL IsComplete();

        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart, BOOL& Consumed );
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd, BOOL& Consumed );
        virtual HRESULT OnText( CXMLText* pText, BOOL& Consumed );
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute, BOOL& Consumed );

    protected:
        CElementNodeCallback();
        virtual ~CElementNodeCallback();

        HRESULT Initialize( CParseContext* pContext, CPropertyObject* pParent, CXMLElementStart* pXMLStart );

        CPropertyObject* m_Element;
        CPropertyNodeCallback* m_ChildNode;
        CResolvedClass* m_ResolvedClass;
        CPropertyInformation* m_Properties;
        BOOL m_Complete;
        CObjectWithType* m_Key;
};