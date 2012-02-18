#pragma once

#include "GradientBrush.h"
#include "Factory.h"
#include "LayeredValue.h"

class CLinearGradientBrush : public CGradientBrush
{
    public:
        DECLARE_FACTORY1( CLinearGradientBrush, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::LinearGradientBrush, CGradientBrush );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual __checkReturn HRESULT GetGraphicsBrush( 
            __in CGraphicsDevice* pGraphicsDevice,
            __in CRenderTarget* pRenderTarget, 
            __deref_out CGraphicsBrush** ppGraphicsBrush 
            );

        //
        // Properties
        //
        static CStaticProperty StartPointProperty;
        static CStaticProperty EndPointProperty;

    protected:
        CLinearGradientBrush(
            );

        virtual ~CLinearGradientBrush(
            );

        __checkReturn HRESULT Initialize( 
            __in CProviders* pProviders 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnStartPointChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnEndPointChanged );

        __checkReturn HRESULT OnStartPointChanged(
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue 
            );

        __checkReturn HRESULT OnEndPointChanged( 
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );

        CLayeredValue m_StartPoint;
        CLayeredValue m_EndPoint;
};

template< >
struct ObjectTypeTraits< CLinearGradientBrush >
{
    static const TypeIndex::Value Type = TypeIndex::LinearGradientBrush;
};