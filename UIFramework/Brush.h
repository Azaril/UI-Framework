#pragma once

#include "PropertyObject.h"
#include "RenderTarget.h"
#include "GraphicsBrush.h"
#include "TypeIndex.h"
#include "GraphicsDevice.h"
#include "Visual.h"

class CBrush : public CRefCountedObjectBase< CPropertyObject >,
               public CVisualResource
{
    public:
        DELEGATE_REFCOUNTING( CRefCountedObjectBase< CPropertyObject > );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Brush, CObjectWithType );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT OnVisualAttach( CVisualAttachContext& Context );
        virtual HRESULT OnVisualDetach( CVisualDetachContext& Context );

        HRESULT InvalidateBrush();

        virtual HRESULT GetGraphicsBrush( CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush ) = 0;

    protected:
        CBrush();
        virtual ~CBrush();

        CPropertyInformation* m_PropertyInformation;
};

template< >
struct ObjectTypeTraits< CBrush >
{
    static const TypeIndex::Value Type = TypeIndex::Brush;
};

class CBrushInvalidatedNotification : public CVisualNotification
{
    public:
        DECLARE_FACTORY1( CBrushInvalidatedNotification, CBrush* );

        virtual VisualNotification::Value GetType();
        CBrush* GetBrush();

    protected:
        CBrushInvalidatedNotification();
        virtual ~CBrushInvalidatedNotification();

        HRESULT Initialize( CBrush* pBrush );

        CBrush* m_Brush;
};