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

        static __checkReturn HRESULT CreatePropertyInformation(
            __deref_out CPropertyInformation** ppInformation 
            );

        //
        // Properties
        //
        static CStaticProperty GradientStopsProperty;

    protected:
        CGradientBrush(
            );

        virtual ~CGradientBrush(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );

        __override virtual __checkReturn HRESULT SetValueInternal( 
            __in CProperty* pProperty,
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetValueInternal( 
            __in CProperty* pProperty,
            __deref_out_opt CObjectWithType** ppValue 
            );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnGradientStopsChanged );

        __checkReturn HRESULT OnGradientStopsChanged(
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );

        CGradientStopCollection* m_GradientStops;
};

template< >
struct ObjectTypeTraits< CGradientBrush >
{
    static const TypeIndex::Value Type = TypeIndex::GradientBrush;
};