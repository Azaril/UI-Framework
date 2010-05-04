#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "UIElement.h"
#include "XMLReader.h"
#include "ClassResolver.h"
#include "TypeConverter.h"

class CParseContext : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CParseContext, CClassResolver*, CTypeConverter* );

        CClassResolver* GetClassResolver();
        CTypeConverter* GetTypeConverter();

    protected:
        CParseContext();
        virtual ~CParseContext();

        HRESULT Initialize( CClassResolver* pClassResolver, CTypeConverter* );

        CClassResolver* m_ClassResolver;
        CTypeConverter* m_TypeConverter;
};

class CParserXMLCallback
{
    virtual HRESULT OnElementStart( CXMLElementStart* pElementStart, BOOL& Consumed ) = 0;
    virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd, BOOL& Consumed ) = 0;
    virtual HRESULT OnText( CXMLText* pText, BOOL& Consumed ) = 0;
    virtual HRESULT OnAttribute( CXMLAttribute* pAttribute, BOOL& Consumed ) = 0;
};

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

class CPropertyCallback : public CParserNodeCallback
{
    public:
};

class CElementNodeCallback : public CParserNodeCallback
{
    public:
        DECLARE_FACTORY3( CElementNodeCallback, CParseContext*, CPropertyObject*, CXMLElementStart* );

        CPropertyObject* GetObject();

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
        CPropertyCallback* m_ChildNode;
        CResolvedClass* m_ResolvedClass;
        CPropertyInformation* m_Properties;
        BOOL m_Complete;
};

class CRichPropertyNodeCallback : public CPropertyCallback
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
        CResolvedClass* m_ResolvedClass;
        CPropertyInformation* m_Properties;
};

class CContentPropertyNodeCallback : public CPropertyCallback
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

class CParserCallback : public CRefCountedObject,
                        public CXMLReaderCallback                        
{
    public:
        DECLARE_FACTORY2( CParserCallback, CClassResolver*, CTypeConverter* );

        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart );
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd );
        virtual HRESULT OnText( CXMLText* pText );
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute );

        virtual HRESULT GetRootElement( CUIElement** ppRootElement );

    protected:
        CParserCallback();
        virtual ~CParserCallback();

        HRESULT Initialize( CClassResolver* pClassResolver, CTypeConverter* );

        CElementNodeCallback* m_ChildNode;
        CParseContext* m_ParseContext;
};

HRESULT ElementStartToParserCallback( CParseContext* pContext, CXMLElementStart* pStart, CElementNodeCallback** ppCallback );
HRESULT ElementStartToParserCallback( CParseContext* pContext, CPropertyObject* pParent, CXMLElementStart* pStart, CElementNodeCallback** ppCallback );
HRESULT ElementStartToParserCallback( CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLElementStart* pStart, CPropertyCallback** ppCallback );
HRESULT TextToParserCallback( CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLText* pText, CPropertyCallback** ppCallback );

HRESULT AssignProperty( CPropertyObject* pElement, CProperty* pProperty, CObjectWithType* pValue, CTypeConverter* pTypeConverter );