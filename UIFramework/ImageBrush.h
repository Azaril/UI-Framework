#pragma once

#include "Brush.h"
#include "BitmapSource.h"
#include "Factory.h"

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
    //typedef std::vector< CImageBrushRenderContext* > RenderTargetContextCollection;

    public:
        DECLARE_FACTORY2( CImageBrushContext, CGraphicsDevice*, CBitmapSource* );

        CGraphicsDevice* GetGraphicsDevice();

        INT32 AddUsage();
        INT32 RemoveUsage();

        //HRESULT AddRenderTarget( CRenderTarget* pRenderTarget );
        //HRESULT RemoveRenderTarget( CRenderTarget* pRenderTarget );

        //HRESULT GetContextForRenderTarget( CRenderTarget* pRenderTarget, CImageBrushRenderContext** ppContext );

        HRESULT GetBitmapSource( CBitmapSource** ppSource );
        HRESULT SetBitmapSource( CBitmapSource* pSource );

    protected:
        CImageBrushContext();
        virtual ~CImageBrushContext();

        HRESULT Initialize( CGraphicsDevice* pGraphicsDevice, CBitmapSource* );

        CGraphicsDevice* m_Device;
        CBitmapSource* m_Source;
        UINT32 m_UsageCount;
        /*RenderTargetContextCollection m_RenderTargetContexts;*/
};

class CImageBrush : public CBrush
{
    typedef std::vector< CImageBrushContext* > ContextCollection;

    public:
        DECLARE_FACTORY( CImageBrush );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ImageBrush, CBrush );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT SetValue( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValue( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT OnVisualAttach( CVisualAttachContext& Context );
        virtual HRESULT OnVisualDetach( CVisualDetachContext& Context );

        HRESULT SetSource( CObjectWithType* pSource );

        virtual HRESULT GetGraphicsBrush( CGraphicsDevice* pDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush );

        virtual HRESULT GetSize( SizeU* pSize );

        //
        // Properties
        //
        static CStaticProperty SourceProperty;

    protected:
        CImageBrush();
        virtual ~CImageBrush();

        HRESULT Initialize();
        HRESULT InternalSetSource( CObjectWithType* m_Source );

        HRESULT EnsureBitmaps();
        HRESULT ReleaseBitmaps();
        HRESULT CreateBitmapFromSource( CGraphicsDevice* pGraphicsDevice, CBitmapSource** ppBitmapSource );

        CObjectWithType* m_Source;
        ContextCollection m_Contexts;
};

template< >
struct ObjectTypeTraits< CImageBrush >
{
    static const TypeIndex::Value Type = TypeIndex::ImageBrush;
};