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
        DECLARE_FACTORY3( CRichPropertyNodeCallback, CParseContext*, CPropertyObject*, CXMLElementStart* );

        virtual BOOL IsComplete();

    protected:
        CRichPropertyNodeCallback();
        virtual ~CRichPropertyNodeCallback();

        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart, BOOL& Consumed );
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd, BOOL& Consumed );
        virtual HRESULT OnText( CXMLText* pText, BOOL& Consumed );
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute, BOOL& Consumed );

        HRESULT Initialize( CParseContext* pContext, CPropertyObject* pParent, CXMLElementStart* pXMLStart );

        CPropertyObject* m_Parent;
        BOOL m_Complete;
        BOOL m_SetTextValue;
        BOOL m_SetObjectValue;
        CElementNodeCallback* m_ChildNode;
        CProperty* m_Property;
};