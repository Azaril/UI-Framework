#pragma once

#include "PropertyObject.h"
#include "Factory.h"
#include "StaticPropertyInformation.h"
#include "Providers.h"
#include "LayeredValue.h"

class CGradientStop : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY1( CGradientStop, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::GradientStop, CPropertyObject );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        //
        // Properties
        //
        static CStaticProperty ColorProperty;
        static CStaticProperty OffsetProperty;

    protected:
        CGradientStop(
            );
    
        virtual ~CGradientStop(
            );

        __checkReturn HRESULT Initialize( 
            CProviders* pProviders 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnColorChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnOffsetChanged );

        __checkReturn HRESULT OnColorChanged(
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue 
            );

        __checkReturn HRESULT OnOffsetChanged( 
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue 
            );

        CLayeredValue m_Color;
        CLayeredValue m_Offset;
};

template< >
struct ObjectTypeTraits< CGradientStop >
{
    static const TypeIndex::Value Type = TypeIndex::GradientStop;
};