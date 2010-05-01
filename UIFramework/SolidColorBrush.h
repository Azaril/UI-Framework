#pragma once

#include "Brush.h"

class CSolidColorBrush : public CBrush
{
    public:
        DECLARE_FACTORY( CSolidColorBrush );

        virtual TypeIndex::Value GetType() { return TypeIndex::SolidColorBrush; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::SolidColorBrush || CBrush::IsTypeOf(Type); }

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT GetGraphicsBrush( CGraphicsDevice* pDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush );

    protected:
        CSolidColorBrush();
        virtual ~CSolidColorBrush();

        HRESULT Initialize();

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        HRESULT InternalSetColor( ColorF Color );

        ColorF m_Color;
};
