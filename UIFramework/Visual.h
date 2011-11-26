#pragma once

#include "Factory.h"
#include "RefCounted.h"
#include "Collections.h"
#include "RenderTarget.h"
#include "RenderContext.h"
#include "GraphicsDevice.h"
#include "VisualResource.h"
#include "HitTestResult.h"
#include "Transform.h"
#include "Animatable.h"

class UIFRAMEWORK_API CVisual : public CAnimatable
{
    typedef vector< CVisual* > VisualChildCollection;
    typedef vector< std::pair< CVisualResource*, OnValueChangeFunc > > VisualResourceCollection;

    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Visual, CAnimatable );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual __out CTimeSource* GetTimeSource(
            );

        __override virtual __checkReturn HRESULT SetAnimationValue(
            __in CProperty* pProperty,
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetAnimationBaseValue(
            __in CProperty* pProperty,
            __deref_out CObjectWithType** ppValue 
            );

        virtual __checkReturn HRESULT PreRender(
            CPreRenderContext& Context 
            );

        virtual __checkReturn HRESULT Render(
            CRenderContext& Context 
            );

        virtual HRESULT GetLocalTransform(
            __out Matrix3X2F* pTransform
            );

        virtual __checkReturn HRESULT HitTest(
            Point2F LocalPoint, 
            __deref_out_opt CHitTestResult** ppHitTestResult 
            ) = 0;

        virtual UINT32 GetVisualChildCount(
            );

        virtual __out CVisual* GetVisualChild( 
            UINT32 Index 
            );

        virtual __checkReturn HRESULT TransformToParent(
            __deref_out CTransform** ppTransform 
            );

        virtual __checkReturn HRESULT TransformFromAncestor( 
            __in_opt CVisual* pAncestor,
            __deref_out CTransform** ppTransform 
            );

        virtual __checkReturn HRESULT TransformToAncestor( 
            __in_opt CVisual* pAncestor,
            __deref_out CTransform** ppTransform 
            );
    
    protected:
        CVisual(
            );

        virtual ~CVisual(
            );
   
        __checkReturn HRESULT Initialize(
            );

        __checkReturn HRESULT Finalize(
            );

        __override virtual __checkReturn HRESULT SetValueInternal( 
            __in CProperty* pProperty, 
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetValueInternal(
            __in CProperty* pProperty, 
            __deref_out_opt CObjectWithType** ppValue 
            );
    
        virtual __checkReturn HRESULT OnVisualAttach(
            CVisualAttachContext& Context 
            );

        virtual __checkReturn HRESULT OnVisualDetach( 
            CVisualDetachContext& Context 
            );

        virtual __checkReturn HRESULT AddChildVisual(
            __in CVisual* pVisualChild 
            );

        virtual __checkReturn HRESULT RemoveChildVisual( 
            __in CVisual* pVisualChild 
            );

        virtual __checkReturn HRESULT MoveToBack(
            __in CVisual* pVisualChild 
            );

        virtual __checkReturn HRESULT MoveToFront( 
            __in CVisual* pVisualChild 
            );

        virtual __checkReturn HRESULT AddVisualResource( 
            __in CVisualResource* pVisualResource,
            __in_opt CProperty* pProperty 
            );

        virtual __checkReturn HRESULT AddVisualResource(
            __in CVisualResource* pVisualResource,
            __in_opt OnValueChangeFunc ChangeFunc 
            );

        virtual __checkReturn HRESULT RemoveVisualResource( 
            __in CVisualResource* pVisualResource, 
            __in_opt CProperty* pProperty 
            );

        virtual __checkReturn HRESULT RemoveVisualResource( 
            __in CVisualResource* pVisualResource,
            __in_opt OnValueChangeFunc ChangeFunc 
            );

        virtual __checkReturn HRESULT RenderTransformed( 
            CRenderContext& Context 
            );

        virtual __checkReturn HRESULT RenderChildren( 
            CRenderContext& Context 
            );

        virtual __out_opt const Matrix3X2F* GetChildRenderTransform(
            );

        __out_opt CVisual* GetVisualParent(
            );

        __checkReturn HRESULT InvalidateVisual(
            );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnChildInvalidated );

        __checkReturn HRESULT OnChildInvalidated(
            __in_opt CObjectWithType* pOldValue, 
            __in_opt CObjectWithType* pNewValue 
            );

        CVisualAttachContext m_VisualContext;
        VisualChildCollection m_VisualChildren;
        VisualResourceCollection m_VisualResources;
        bool m_VisualAttached;
};

template< >
struct ObjectTypeTraits< CVisual >
{
    static const TypeIndex::Value Type = TypeIndex::Visual;
};