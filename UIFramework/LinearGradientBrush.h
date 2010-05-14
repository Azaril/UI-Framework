#pragma once

#include "GradientBrush.h"
#include "Factory.h"

class CLinearGradientBrush : public CGradientBrush
{
    public:
        DECLARE_FACTORY1( CLinearGradientBrush, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::LinearGradientBrush, CGradientBrush );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT GetGraphicsBrush( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush );

        //
        // Properties
        //
        static CStaticProperty StartPointProperty;
        static CStaticProperty EndPointProperty;

    protected:
        CLinearGradientBrush();
        virtual ~CLinearGradientBrush();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnStartPointChanged );
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnEndPointChanged );

        HRESULT OnStartPointChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );
        HRESULT OnEndPointChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        Point2F m_StartPoint;
        Point2F m_EndPoint;
};

template< >
struct ObjectTypeTraits< CLinearGradientBrush >
{
    static const TypeIndex::Value Type = TypeIndex::LinearGradientBrush;
};