#pragma once

#include "Brush.h"
#include "Providers.h"
#include "LayeredValue.h"

class CSolidColorBrush : public CBrush
{
    public:
        DECLARE_FACTORY1( CSolidColorBrush, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::SolidColorBrush, CBrush );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual __checkReturn HRESULT GetGraphicsBrush( 
            __in CGraphicsDevice* pDevice, 
            __in CRenderTarget* pRenderTarget,
            __deref_out CGraphicsBrush** ppGraphicsBrush 
            );

        //
        // Properties
        //
        static CStaticProperty ColorProperty;

    protected:
        CSolidColorBrush(
            );

        virtual ~CSolidColorBrush(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders 
            );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnColorChanged );

        __checkReturn HRESULT OnColorChanged( 
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue 
            );

        CLayeredValue m_Color;
};

template< >
struct ObjectTypeTraits< CSolidColorBrush >
{
    static const TypeIndex::Value Type = TypeIndex::SolidColorBrush;
};