#pragma once

#include "PropertyObject.h"
#include "Factory.h"
#include "StaticPropertyInformation.h"
#include "Providers.h"

class CGradientStop : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY1( CGradientStop, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::GradientStop, CPropertyObject );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        ColorF GetColor(
            );

        FLOAT GetOffset(
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

        virtual __checkReturn HRESULT SetValueInternal(
            __in CProperty* pProperty, 
            __in CObjectWithType* pValue 
            );

        virtual __checkReturn HRESULT GetValueInternal(
            __in CProperty* pProperty,
            __deref_out CObjectWithType** ppValue 
            );

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

        ColorF m_Color;
        FLOAT m_Offset;
};

template< >
struct ObjectTypeTraits< CGradientStop >
{
    static const TypeIndex::Value Type = TypeIndex::GradientStop;
};