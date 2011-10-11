#pragma once

#include "Brush.h"
#include "BitmapSource.h"
#include "Factory.h"
#include "Providers.h"

//class CImageBrushRenderContext : public CRefCountedObject
//{
//    public:
//        DECLARE_FACTORY1( CImageBrushRenderContext, CRenderTarget* );
//
//        CRenderTarget* GetRenderTarget();
//
//        INT32 AddUsage();
//        INT32 RemoveUsage();
//
//        HRESULT GetGraphicsBrush( CGraphicsBrush** ppBrush );
//        HRESULT SetGraphicsBrush( CGraphicsBrush* pBrush );
//
//    protected:
//        CImageBrushRenderContext();
//        virtual ~CImageBrushRenderContext();
//
//        HRESULT Initialize( CRenderTarget* pRenderTarget );
//
//        CRenderTarget* m_RenderTarget;
//        CGraphicsBrush* m_GraphicsBrush;
//        UINT32 m_UsageCount;
//};

class CImageBrushContext : public CRefCountedObject
{
    //typedef vector< CImageBrushRenderContext* > RenderTargetContextCollection;

    public:
        DECLARE_FACTORY2( CImageBrushContext, CGraphicsDevice*, CBitmapSource* );

        __out CGraphicsDevice* GetGraphicsDevice(
            );

        INT32 AddUsage(
            );

        INT32 RemoveUsage(
            );

        //HRESULT AddRenderTarget( CRenderTarget* pRenderTarget );
        //HRESULT RemoveRenderTarget( CRenderTarget* pRenderTarget );

        //HRESULT GetContextForRenderTarget( CRenderTarget* pRenderTarget, CImageBrushRenderContext** ppContext );

        __checkReturn HRESULT GetBitmapSource( 
            __deref_out CBitmapSource** ppSource 
            );

        __checkReturn HRESULT SetBitmapSource( 
            __in CBitmapSource* pSource 
            );

    protected:
        CImageBrushContext(
            );

        virtual ~CImageBrushContext(
            );

        __checkReturn HRESULT Initialize( 
            __in CGraphicsDevice* pGraphicsDevice, 
            __in CBitmapSource* 
            );

        CGraphicsDevice* m_Device;
        CBitmapSource* m_Source;
        UINT32 m_UsageCount;
        /*RenderTargetContextCollection m_RenderTargetContexts;*/
};

class CImageBrush : public CBrush
{
    typedef vector< CImageBrushContext* > ContextCollection;

    public:
        DECLARE_FACTORY1( CImageBrush, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ImageBrush, CBrush );

        static __checkReturn HRESULT CreatePropertyInformation( 
            __deref_out CPropertyInformation** ppInformation 
            );

        __override virtual __checkReturn HRESULT OnVisualAttach(
            CVisualAttachContext& Context 
            );

        __override virtual __checkReturn HRESULT OnVisualDetach( 
            CVisualDetachContext& Context 
            );

        __checkReturn HRESULT SetSource( 
            __in_opt CObjectWithType* pSource 
            );

        __override virtual __checkReturn HRESULT GetGraphicsBrush(
            __in CGraphicsDevice* pDevice, 
            __in CRenderTarget* pRenderTarget, 
            __deref_out CGraphicsBrush** ppGraphicsBrush 
            );

        __override virtual __checkReturn HRESULT GetSize(
            __out SizeU* pSize 
            );

        //
        // Properties
        //
        static CStaticProperty SourceProperty;

    protected:
        CImageBrush(
            );

        virtual ~CImageBrush(
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

        __checkReturn HRESULT InternalSetSource( 
            __in_opt CObjectWithType* m_Source 
            );

        __checkReturn HRESULT EnsureBitmaps(
            );

        __checkReturn HRESULT ReleaseBitmaps(
            );

        __checkReturn HRESULT CreateBitmapFromSource( 
            __in CGraphicsDevice* pGraphicsDevice, 
            __deref_out CBitmapSource** ppBitmapSource 
            );

        CObjectWithType* m_Source;
        ContextCollection m_Contexts;
        CProviders* m_pProviders;
};

template< >
struct ObjectTypeTraits< CImageBrush >
{
    static const TypeIndex::Value Type = TypeIndex::ImageBrush;
};