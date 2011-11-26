#pragma once

#include "PropertyObject.h"
#include "RenderTarget.h"
#include "GraphicsBrush.h"
#include "TypeIndex.h"
#include "GraphicsDevice.h"
#include "Visual.h"
#include "StaticPropertyInformation.h"

class CBrush : public CVisualResource
{
    public:
        DELEGATE_REFCOUNTING( CRefCountedObjectBase< CPropertyObject > );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Brush, CPropertyObject );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );
        
        __override virtual bool IsShareable(
            )
        { 
            return TRUE; 
        }

        __override virtual __checkReturn HRESULT OnVisualAttach( 
            CVisualAttachContext& Context 
            );

        __override virtual __checkReturn HRESULT OnVisualDetach(
            CVisualDetachContext& Context 
            );

        virtual __checkReturn HRESULT GetGraphicsBrush( 
            __in CGraphicsDevice* pGraphicsDevice, 
            __in CRenderTarget* pRenderTarget, 
            __deref_out CGraphicsBrush** ppGraphicsBrush 
            ) = 0;

    protected:
        CBrush(
            );

        virtual ~CBrush(
            );

        __override virtual __checkReturn HRESULT SetValueInternal(
            __in CProperty* pProperty, 
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetValueInternal(
            __in CProperty* pProperty,
            __deref_out_opt CObjectWithType** ppValue 
            );

        CPropertyInformation* m_PropertyInformation;
};

template< >
struct ObjectTypeTraits< CBrush >
{
    static const TypeIndex::Value Type = TypeIndex::Brush;
};