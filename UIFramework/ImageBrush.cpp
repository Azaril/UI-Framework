#include "ImageBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

CStaticProperty ImageBrushProperties[] = 
{
    CStaticProperty( L"Source", TypeIndex::Object, StaticPropertyFlags::None )
};

namespace ImageBrushPropertyIndex
{
    enum Value
    {
        Source
    };
}

CImageBrush::CImageBrush() : m_Source(NULL)
{
}

CImageBrush::~CImageBrush()
{
    ReleaseObject(m_Source);
}

HRESULT CImageBrush::Initialize()
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CImageBrush::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(ImageBrushProperties, ARRAYSIZE(ImageBrushProperties), &pStaticInformation));
    IFC(CBrush::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CImageBrush::SetSource(CObjectWithType* pSource)
{
    HRESULT hr = S_OK;

    IFC(InternalSetSource(pSource));

Cleanup:
    return hr;
}

HRESULT CImageBrush::InternalSetSource(CObjectWithType* pSource)
{
    HRESULT hr = S_OK;

    IFCEXPECT(pSource->IsTypeOf(TypeIndex::String) || pSource->IsTypeOf(TypeIndex::BitmapSource));

    ReleaseObject(m_Source);

    m_Source = pSource;

    AddRefObject(m_Source);

Cleanup:
    return hr;
}

HRESULT CImageBrush::OnVisualAttach(CVisualAttachContext& Context)
{
    HRESULT hr = S_OK;
    CImageBrushContext* pContext = NULL;
    CImagingProvider* pImagingProvider = NULL;
    CBitmapSource* pBitmapSource = NULL;

    for(ContextCollection::iterator It = m_Contexts.begin(); It != m_Contexts.end(); ++It)
    {
        if((*It)->GetGraphicsDevice() == Context.GetGraphicsDevice())
        {
            pContext = *It;
            break;
        }
    }

    if(pContext != NULL)
    {
        pContext->AddUsage();

        pContext = NULL;
    }
    else
    {
        if(m_Source->IsTypeOf(TypeIndex::String))
        {
            CStringValue* pStringValue = (CStringValue*)m_Source;

            IFC(Context.GetGraphicsDevice()->GetImagingProvider(&pImagingProvider));

            IFC(pImagingProvider->LoadBitmapFromFile(pStringValue->GetValue(), &pBitmapSource));
        }
        else if(m_Source->IsTypeOf(TypeIndex::BitmapSource))
        {
            pBitmapSource = (CBitmapSource*)m_Source;
            AddRefObject(m_Source);
        }
        else
        {
            IFC(E_FAIL);
        }

        IFC(CImageBrushContext::Create(Context.GetGraphicsDevice(), pBitmapSource, &pContext));

        m_Contexts.push_back(pContext);

        pContext = NULL;
    }

Cleanup:
    ReleaseObject(pContext);
    ReleaseObject(pImagingProvider);

    return hr;
}

HRESULT CImageBrush::OnVisualDetach(CVisualDetachContext& Context)
{
    HRESULT hr = S_OK;

    for(ContextCollection::iterator It = m_Contexts.begin(); It != m_Contexts.end(); ++It)
    {
        if((*It)->GetGraphicsDevice() == Context.GetGraphicsDevice())
        {
            if((*It)->RemoveUsage() == 0)
            {
                (*It)->Release();

                m_Contexts.erase(It);
            }
            
            goto Cleanup;
        }
    }

    IFCPTR(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CImageBrush::GetGraphicsBrush(CGraphicsDevice* pGraphicsDevice, CRenderTarget* pRenderTarget, CGraphicsBrush** ppGraphicsBrush)
{
    HRESULT hr = S_OK;
    CImageBrushContext* pContext = NULL;
    CBitmap* pBitmap = NULL;
    CBitmapSource* pBitmapSource = NULL;
    CImagingProvider* pImagingProvider = NULL;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(ppGraphicsBrush);

    for(ContextCollection::iterator It = m_Contexts.begin(); It != m_Contexts.end(); ++It)
    {
        if((*It)->GetGraphicsDevice() == pGraphicsDevice)
        {
            pContext = *It;
            break;
        }
    }

    IFCPTR(pContext);

    IFC(pContext->GetBitmapSource(&pBitmapSource));

    //if(pBitmapSource == NULL)
    //{
    //    IFCPTR(m_Source);

    //    if(m_Source->IsTypeOf(TypeIndex::String))
    //    {
    //        CStringValue* pStringValue = (CStringValue*)m_Source;

    //        IFC(pGraphicsDevice->GetImagingProvider(&pImagingProvider));

    //        IFC(pImagingProvider->LoadBitmapFromFile(pStringValue->GetValue(), &pBitmapSource));
    //    }
    //    else
    //    {
    //        IFC(E_FAIL);
    //    }

    //    IFCPTR(pBitmapSource);

    //    IFC(pContext->SetBitmapSource(pBitmapSource));
    //}

    //TODO: Find a way to keep a single instance per render target.
    IFC(pRenderTarget->LoadBitmap(pBitmapSource, &pBitmap));

    IFC(pRenderTarget->CreateBitmapBrush(pBitmap, ppGraphicsBrush));

Cleanup:
    ReleaseObject(pBitmap);
    ReleaseObject(pBitmapSource);
    ReleaseObject(pImagingProvider);

    return hr;
}

HRESULT CImageBrush::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= ImageBrushProperties && pProperty < ImageBrushProperties + ARRAYSIZE(ImageBrushProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - ImageBrushProperties);
        
        switch(Index)
        {
            case ImageBrushPropertyIndex::Source:
                {
                    IFC(InternalSetSource(pValue));

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CBrush::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CImageBrush::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    if(wcscmp(pProperty->GetName(), L"Source") == 0)
    {
        *ppValue = m_Source;
        AddRefObject(m_Source);
    }
    else
    {
        IFC(CBrush::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

HRESULT CImageBrush::GetSize(SizeU* pSize)
{
    HRESULT hr = S_OK;
    CBitmapSource* pBitmapSource = NULL;

    IFCPTR(pSize);

    for(ContextCollection::iterator It = m_Contexts.begin(); It != m_Contexts.end(); ++It)
    {
        IFC((*It)->GetBitmapSource(&pBitmapSource));

        if(pBitmapSource)
        {
            IFC(pBitmapSource->GetSize(pSize));

            goto Cleanup;
        }
    }

    pSize->height = 0;
    pSize->width = 0;

Cleanup:
    ReleaseObject(pBitmapSource);

    return hr;
}




//CImageBrushRenderContext::CImageBrushRenderContext() : m_RenderTarget(NULL),
//                                                       m_GraphicsBrush(NULL),
//                                                       m_UsageCount(1)
//{
//}
//
//CImageBrushRenderContext::~CImageBrushRenderContext()
//{
//    ReleaseObject(m_RenderTarget);
//    ReleaseObject(m_GraphicsBrush);
//}
//
//HRESULT CImageBrushRenderContext::Initialize(CRenderTarget* pRenderTarget)
//{
//    HRESULT hr = S_OK;
//
//    IFCPTR(pRenderTarget);
//
//    m_RenderTarget = pRenderTarget;
//    AddRefObject(m_RenderTarget);
//
//Cleanup:
//    return hr;
//}
//
//CRenderTarget* CImageBrushRenderContext::GetRenderTarget()
//{
//    return m_RenderTarget;
//}
//
//INT32 CImageBrushRenderContext::AddUsage()
//{
//    return ++m_UsageCount;
//}
//
//INT32 CImageBrushRenderContext::RemoveUsage()
//{
//    return --m_UsageCount;
//}
//
//HRESULT CImageBrushRenderContext::GetGraphicsBrush(CGraphicsBrush** ppBrush)
//{
//    HRESULT hr = S_OK;
//
//    IFCPTR(ppBrush);
//
//    *ppBrush = m_GraphicsBrush;
//    AddRefObject(m_GraphicsBrush);
//
//Cleanup:
//    return hr;
//}
//
//HRESULT CImageBrushRenderContext::SetGraphicsBrush(CGraphicsBrush* pBrush)
//{
//    HRESULT hr = S_OK;
//
//    IFCPTR(pBrush);
//
//    m_GraphicsBrush = pBrush;
//    AddRefObject(m_GraphicsBrush);
//
//Cleanup:
//    return hr;
//}



CImageBrushContext::CImageBrushContext() : m_Device(NULL),
                                           m_Source(NULL),
                                           m_UsageCount(1)
{
}

CImageBrushContext::~CImageBrushContext()
{
    ReleaseObject(m_Device);
    ReleaseObject(m_Source);

    //for(RenderTargetContextCollection::iterator It = m_RenderTargetContexts.begin(); It != m_RenderTargetContexts.end(); ++It)
    //{
    //    (*It)->Release();
    //}

    //m_RenderTargetContexts.clear();
}

HRESULT CImageBrushContext::Initialize(CGraphicsDevice* pGraphicsDevice, CBitmapSource* pBitmapSource)
{
    HRESULT hr = S_OK;

    IFCPTR(pGraphicsDevice);

    m_Device = pGraphicsDevice;
    AddRefObject(m_Device);
    
    m_Source = pBitmapSource;
    AddRefObject(m_Source);

Cleanup:
    return hr;
}

CGraphicsDevice* CImageBrushContext::GetGraphicsDevice()
{
    return m_Device;
}

INT32 CImageBrushContext::AddUsage()
{
    return ++m_UsageCount;
}

INT32 CImageBrushContext::RemoveUsage()
{
    return --m_UsageCount;
}

HRESULT CImageBrushContext::GetBitmapSource( CBitmapSource** ppSource )
{
    HRESULT hr = S_OK;

    IFCPTR(ppSource);

    *ppSource = m_Source;
    AddRefObject(m_Source);

Cleanup:
    return hr;
}

HRESULT CImageBrushContext::SetBitmapSource(CBitmapSource* pSource)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_Source);

    m_Source = pSource;

    AddRefObject(m_Source);

    return hr;
}

//HRESULT CImageBrushContext::AddRenderTarget(CRenderTarget* pRenderTarget)
//{
//    HRESULT hr = S_OK;
//    CImageBrushRenderContext* pRenderTargetContext = NULL;
//
//    IFCPTR(pRenderTarget);
//
//    for(RenderTargetContextCollection::iterator It = m_RenderTargetContexts.begin(); It != m_RenderTargetContexts.end(); ++It)
//    {
//        if((*It)->GetRenderTarget() == pRenderTarget)
//        {
//            (*It)->AddUsage();
//
//            goto Cleanup;
//        }
//    }
//
//    IFC(CImageBrushRenderContext::Create(pRenderTarget, &pRenderTargetContext));
//
//    m_RenderTargetContexts.push_back(pRenderTargetContext);
//
//    pRenderTargetContext = NULL;
//
//Cleanup:
//    ReleaseObject(pRenderTargetContext);
//
//    return hr;
//}
//
//HRESULT CImageBrushContext::RemoveRenderTarget(CRenderTarget* pRenderTarget)
//{
//    HRESULT hr = S_OK;
//
//    IFCPTR(pRenderTarget);
//
//    for(RenderTargetContextCollection::iterator It = m_RenderTargetContexts.begin(); It != m_RenderTargetContexts.end(); ++It)
//    {
//        if((*It)->GetRenderTarget() == pRenderTarget)
//        {
//            if((*It)->RemoveUsage() == 0)
//            {
//                (*It)->Release();
//
//                m_RenderTargetContexts.erase(It);
//            }
//
//            goto Cleanup;
//        }
//    }
//
//    IFC(E_FAIL);
//
//Cleanup:
//    return hr;
//}

//HRESULT CImageBrushContext::GetContextForRenderTarget(CRenderTarget* pRenderTarget, CImageBrushRenderContext** ppContext)
//{
//    HRESULT hr = S_OK;
//
//    IFCPTR(ppContext);
//
//    for(RenderTargetContextCollection::iterator It = m_RenderTargetContexts.begin(); It != m_RenderTargetContexts.end(); ++It)
//    {
//        if((*It)->GetRenderTarget() == pRenderTarget)
//        {
//            *ppContext = (*It);
//            (*It)->AddRef();
//
//            goto Cleanup;
//        }
//    }
//
//    IFC(E_FAIL);
//
//Cleanup:
//    return hr;
//}