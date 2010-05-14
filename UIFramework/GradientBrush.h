#pragma once

#include "Brush.h"
#include "GradientStop.h"
#include "Collection.h"
#include "Providers.h"

class CGradientStopCollection : public CCollection< CGradientStop >
{
    public:
        DECLARE_FACTORY( CGradientStopCollection );

        DECLARE_TYPE_WITH_BASE( TypeIndex::GradientStopCollection, CCollection< CGradientStop > );
};

template< >
struct ObjectTypeTraits< CGradientStopCollection >
{
    static const TypeIndex::Value Type = TypeIndex::GradientStopCollection;
};

class CGradientBrush : public CBrush
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::GradientBrush, CBrush );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        //
        // Properties
        //
        static CStaticProperty GradientStopsProperty;

    protected:
        CGradientBrush();
        virtual ~CGradientBrush();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnGradientStopsChanged );

        HRESULT OnGradientStopsChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CGradientStopCollection* m_GradientStops;
};

template< >
struct ObjectTypeTraits< CGradientBrush >
{
    static const TypeIndex::Value Type = TypeIndex::GradientBrush;
};