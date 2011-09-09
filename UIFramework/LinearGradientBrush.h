#pragma once

#include "GradientBrush.h"
#include "Factory.h"

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

        __override virtual __checkReturn HRESULT SetValueInternal(
            __in CProperty* pProperty,
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetValueInternal(
            __in CProperty* pProperty, 
            __deref_out CObjectWithType** ppValue 
            );

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

        Point2F m_StartPoint;
        Point2F m_EndPoint;
};

template< >
struct ObjectTypeTraits< CLinearGradientBrush >
{
    static const TypeIndex::Value Type = TypeIndex::LinearGradientBrush;
};