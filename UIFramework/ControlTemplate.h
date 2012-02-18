#pragma once

#include "PropertyObject.h"
#include "RefCounted.h"
#include "Factory.h"
#include "ParserCommandList.h"
#include "StaticPropertyInformation.h"
#include "Namescope.h"
#include "LayeredValue.h"

class CControlTemplate : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY1( CControlTemplate, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ControlTemplate, CPropertyObject );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual bool IsShareable(
            ) 
        { 
            return TRUE;
        }

        __checkReturn HRESULT LoadContent(
            __in CNamescope* pNamescope, 
            __deref_out CObjectWithType** ppObject 
            );

       //
        // Properties
        //
        static CStaticProperty TemplateProperty;

    protected:
        CControlTemplate(
            );

        virtual ~CControlTemplate(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        CLayeredValue m_TemplateCommandList;
};

template< >
struct ObjectTypeTraits< CControlTemplate >
{
    static const TypeIndex::Value Type = TypeIndex::ControlTemplate;
};

class CControlTemplateParseCallback : public IParserCallback
{
    public:
        CControlTemplateParseCallback(
            __in CNamescope* pNamescope
            );

        __override virtual __checkReturn HRESULT OnPushObject( 
            __in CObjectWithType* pObject 
            );

        __override virtual __checkReturn HRESULT OnPopObject(
            __in CObjectWithType* pObject 
            );

    protected:
        CNamescope* m_Namescope;
};