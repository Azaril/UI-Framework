#pragma once

#include "Brush.h"

class CSolidColorBrush : public CBrush
{
    public:
        DECLARE_FACTORY( CSolidColorBrush );

        DECLARE_TYPE_WITH_BASE( TypeIndex::SolidColorBrush, CBrush );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT GetGraphicsBrush( CGraphicsDevice* pDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush );

        //
        // Properties
        //
        static CStaticProperty ColorProperty;

    protected:
        CSolidColorBrush();
        virtual ~CSolidColorBrush();

        HRESULT Initialize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );

        HRESULT InternalSetColor( ColorF Color );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnColorChanged );

        HRESULT OnColorChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        ColorF m_Color;
};

template< >
struct ObjectTypeTraits< CSolidColorBrush >
{
    static const TypeIndex::Value Type = TypeIndex::SolidColorBrush;
};