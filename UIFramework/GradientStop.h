#pragma once

#include "PropertyObject.h"
#include "Factory.h"
#include "StaticPropertyInformation.h"

class CGradientStop : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        DECLARE_FACTORY( CGradientStop );

        DECLARE_TYPE_WITH_BASE( TypeIndex::GradientStop, CPropertyObject );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        ColorF GetColor();
        FLOAT GetOffset();

        //
        // Properties
        //
        static CStaticProperty ColorProperty;
        static CStaticProperty OffsetProperty;

    protected:
        CGradientStop();
        virtual ~CGradientStop();

        HRESULT Initialize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnColorChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnOffsetChanged );

        HRESULT OnColorChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnOffsetChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        ColorF m_Color;
        FLOAT m_Offset;
};

template< >
struct ObjectTypeTraits< CGradientStop >
{
    static const TypeIndex::Value Type = TypeIndex::GradientStop;
};