#pragma once

#include "PropertyObject.h"
#include "MarkupExtension.h"
#include "Factory.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

class CTypeMarkupExtension : public CMarkupExtension
{
    public:
        DECLARE_FACTORY1( CTypeMarkupExtension, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TypeMarkupExtension, CMarkupExtension );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT ExecuteMarkup( CObjectWithType** ppObject );

        //
        // Properties
        //
        static CStaticProperty TypeNameProperty;

    protected:
        CTypeMarkupExtension();
        virtual ~CTypeMarkupExtension();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        CStringValue* m_TypeName;
        CProviders* m_Providers;
};

template< >
struct ObjectTypeTraits< CTypeMarkupExtension >
{
    static const TypeIndex::Value Type = TypeIndex::TypeMarkupExtension;
};