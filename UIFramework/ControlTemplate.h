#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Factory.h"
#include "ParserCommandList.h"
#include "StaticPropertyInformation.h"

class CControlTemplate : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY1( CControlTemplate, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ControlTemplate, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        HRESULT LoadContent( CObjectWithType** ppObject );

       //
        // Properties
        //
        static CStaticProperty TemplateProperty;

    protected:
        CControlTemplate();
        virtual ~CControlTemplate();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        CParserCommandList* m_TemplateCommandList;
};

template< >
struct ObjectTypeTraits< CControlTemplate >
{
    static const TypeIndex::Value Type = TypeIndex::ControlTemplate;
};