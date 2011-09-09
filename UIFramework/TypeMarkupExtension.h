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

        static __checkReturn HRESULT CreatePropertyInformation( 
			__deref_out CPropertyInformation** ppInformation 
			);

        __override virtual __checkReturn HRESULT GetValue( 
			__in CProperty* pProperty, 
			__deref_out_opt CObjectWithType** ppValue 
			);

        __override virtual __checkReturn HRESULT ExecuteMarkup(
			__deref_out CObjectWithType** ppObject 
			);

        //
        // Properties
        //
        static CStaticProperty TypeNameProperty;

    protected:
        CTypeMarkupExtension(
			);

        virtual ~CTypeMarkupExtension(
			);

        __checkReturn HRESULT Initialize( 
			__in CProviders* pProviders 
			);

        __override virtual HRESULT SetValueInternal( 
			__in CProperty* pProperty, 
			__in CObjectWithType* pValue 
			);

        CStringValue* m_TypeName;
        CProviders* m_Providers;
};

template< >
struct ObjectTypeTraits< CTypeMarkupExtension >
{
    static const TypeIndex::Value Type = TypeIndex::TypeMarkupExtension;
};