#pragma once

#include "PropertyObject.h"
#include "RenderTarget.h"
#include "GraphicsBrush.h"
#include "TypeIndex.h"
#include "GraphicsDevice.h"
#include "Visual.h"

class CBrush : public CRefCountedObject,
               public CPropertyObject,
               public CVisualResource
{
    public:
        DELEGATE_REFCOUNTING( CRefCountedObject );

        virtual TypeIndex::Value GetType() { return TypeIndex::Brush; }
        virtual BOOL IsTypeOf( TypeIndex::Value Type ) { return Type == TypeIndex::Brush; }

        virtual HRESULT GetPropertyInformation( CPropertyInformation** ppInformation );
        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT OnVisualAttach( CVisualAttachContext& Context );
        virtual HRESULT OnVisualDetach( CVisualDetachContext& Context );

        virtual HRESULT GetGraphicsBrush( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush ) = 0;

    protected:
        CBrush();
        virtual ~CBrush();

        virtual HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        CPropertyInformation* m_PropertyInformation;
};