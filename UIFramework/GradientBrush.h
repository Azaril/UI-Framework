#pragma once

#include "Brush.h"
#include "GradientStop.h"
#include "Collection.h"
#include "Providers.h"
#include "LayeredValue.h"

class CGradientStopCollection : public CCollection< CGradientStop >
{
    public:
        DECLARE_FACTORY1( CGradientStopCollection, CProviders* );

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

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnGradientStopsChanged );

        __checkReturn HRESULT OnGradientStopsChanged(
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );

        CLayeredValue m_GradientStops;
};

template< >
struct ObjectTypeTraits< CGradientBrush >
{
    static const TypeIndex::Value Type = TypeIndex::GradientBrush;
};