#include "ImageBrush.h"
#include "DelegatingPropertyInformation.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"
#include "ResourceProvider.h"

//
// Properties
//
namespace ImageBrushProperties
{
    enum Value
    {
        Source
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Source );

//
// Properties
//
CStaticProperty CImageBrush::SourceProperty(TypeIndex::ImageBrush, ImageBrushProperties::Source, L"Source", TypeIndex::Object, StaticPropertyFlags::None, &GET_DEFAULT( Source ), &INSTANCE_CHANGE_CALLBACK( CImageBrush, OnSourceChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CImageBrush, OnSourceChanged );

CImageBrush::CImageBrush(
    )
{
}

CImageBrush::~CImageBrush(
    )
{
    ReleaseObject(m_pProviders);
}

__checkReturn HRESULT 
CImageBrush::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pProviders, pProviders);

    return hr;
}

__checkReturn HRESULT 
CImageBrush::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &SourceProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CBrush::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

__override __checkReturn HRESULT 
CImageBrush::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::ImageBrush)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case ImageBrushProperties::Source:
                {
                    *ppLayeredValue = &m_Source;
                    break;
                }

            default:
                {
                    IFC(E_UNEXPECTED);
                }
        }
    }
    else
    {
        IFC_NOTRACE(CBrush::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CImageBrush::OnSourceChanged(
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue
    )
{
    HRESULT hr = S_OK;

    IFC(EnsureBitmaps());

    IFC(InvalidateVisualResource());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CImageBrush::OnVisualAttach(
    CVisualAttachContext& Context
    )
{
    HRESULT hr = S_OK;
    CImageBrushContext* pContext = NULL;
    CBitmapSource* pBitmapSource = NULL;

    IFC(CBrush::OnVisualAttach(Context));

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
        IFC(CreateBitmapFromSource(Context.GetGraphicsDevice(), &pBitmapSource));

        IFC(CImageBrushContext::Create(Context.GetGraphicsDevice(), pBitmapSource, &pContext));

        m_Contexts.push_back(pContext);

        pContext = NULL;
    }

Cleanup:
    ReleaseObject(pBitmapSource);
    ReleaseObject(pContext);

    return hr;
}

__override __checkReturn HRESULT
CImageBrush::OnVisualDetach(
    CVisualDetachContext& Context
    )
{
    HRESULT hr = S_OK;

    IFC(CBrush::OnVisualDetach(Context));

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

    IFC(E_FAIL);

Cleanup:
    return hr;
}

__checkReturn HRESULT
CImageBrush::ReleaseBitmaps(
    )
{
    HRESULT hr = S_OK;

    for(ContextCollection::iterator It = m_Contexts.begin(); It != m_Contexts.end(); ++It)
    {
        IFC((*It)->SetBitmapSource(NULL));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CImageBrush::EnsureBitmaps(
    )
{
    HRESULT hr = S_OK;
    CBitmapSource* pBitmapSource = NULL;

    for(ContextCollection::iterator It = m_Contexts.begin(); It != m_Contexts.end(); ++It)
    {
        IFC((*It)->GetBitmapSource(&pBitmapSource));

        if(pBitmapSource == NULL)
        {
            IFC(CreateBitmapFromSource((*It)->GetGraphicsDevice(), &pBitmapSource));

            IFC((*It)->SetBitmapSource(pBitmapSource));
        }
    }

Cleanup:
    ReleaseObject(pBitmapSource);

    return hr;
}

__checkReturn HRESULT 
CImageBrush::CreateBitmapFromSource(
    __in CGraphicsDevice* pGraphicsDevice, 
    __deref_out CBitmapSource** ppBitmapSource
    )
{
    HRESULT hr = S_OK;
    CBitmapSource* pBitmapSource = NULL;
    CImagingProvider* pImagingProvider = NULL;
    IReadStream* pResourceStream = NULL;
    CObjectWithType* pSource = NULL;

    IFCPTR(ppBitmapSource);

    IFC(GetEffectiveValue(&SourceProperty, &pSource));

    if(pSource == NULL)
    {
        pBitmapSource = NULL;
    }
    else if(pSource->IsTypeOf(TypeIndex::String))
    {
        CStringValue* pStringValue = (CStringValue*)pSource;
        IResourceProvider* pResourceProvider = m_pProviders->GetResourceProvider();

        IFC(pResourceProvider->ReadResource(pStringValue->GetValue(), pStringValue->GetLength(), &pResourceStream));

        IFC(pGraphicsDevice->GetImagingProvider(&pImagingProvider));

        IFC(pImagingProvider->LoadBitmapFromStream(pResourceStream, &pBitmapSource));
    }
    else if(pSource->IsTypeOf(TypeIndex::BitmapSource))
    {
        pBitmapSource = (CBitmapSource*)pSource;
        AddRefObject(pBitmapSource);
    }
    else
    {
        IFC(E_FAIL);
    }

    *ppBitmapSource = pBitmapSource;
    pBitmapSource = NULL;

Cleanup:
    ReleaseObject(pBitmapSource);
    ReleaseObject(pImagingProvider);
    ReleaseObject(pResourceStream);
    ReleaseObject(pSource);

    return hr;
}

__checkReturn HRESULT 
CImageBrush::GetGraphicsBrush(
    __in CGraphicsDevice* pGraphicsDevice, 
    __in CRenderTarget* pRenderTarget, 
    __deref_out CGraphicsBrush** ppGraphicsBrush
    )
{
    HRESULT hr = S_OK;
    CImageBrushContext* pContext = NULL;
    CBitmap* pBitmap = NULL;
    CBitmapSource* pBitmapSource = NULL;
    CImagingProvider* pImagingProvider = NULL;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(ppGraphicsBrush);

    //TODO: Fix this to only load the bitmap for the current graphics device.
    IFC(EnsureBitmaps());

    for(ContextCollection::iterator It = m_Contexts.begin(); It != m_Contexts.end(); ++It)
    {
        if((*It)->GetGraphicsDevice() == pGraphicsDevice)
        {
            pContext = *It;
            break;
        }
    }

    IFCPTR_NOTRACE(pContext);

    IFC(pContext->GetBitmapSource(&pBitmapSource));

    IFCPTR_NOTRACE(pBitmapSource);

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
    IFC_NOTRACE(pRenderTarget->LoadBitmap(pBitmapSource, &pBitmap));

    IFC(pRenderTarget->CreateBitmapBrush(pBitmap, ppGraphicsBrush));

Cleanup:
    ReleaseObject(pBitmap);
    ReleaseObject(pBitmapSource);
    ReleaseObject(pImagingProvider);

    return hr;
}

__override __checkReturn HRESULT 
CImageBrush::GetSize(
    __out SizeU* pSize
    )
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



CImageBrushContext::CImageBrushContext(
    ) 
    : m_Device(NULL)
    , m_Source(NULL)
    , m_UsageCount(1)
{
}

CImageBrushContext::~CImageBrushContext(
    )
{
    ReleaseObject(m_Device);
    ReleaseObject(m_Source);

    //for(RenderTargetContextCollection::iterator It = m_RenderTargetContexts.begin(); It != m_RenderTargetContexts.end(); ++It)
    //{
    //    (*It)->Release();
    //}

    //m_RenderTargetContexts.clear();
}

__checkReturn HRESULT
CImageBrushContext::Initialize(
    __in CGraphicsDevice* pGraphicsDevice,
    __in CBitmapSource* pBitmapSource
    )
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

__out CGraphicsDevice* 
CImageBrushContext::GetGraphicsDevice(
    )
{
    return m_Device;
}

INT32 
CImageBrushContext::AddUsage(
    )
{
    return ++m_UsageCount;
}

INT32 
CImageBrushContext::RemoveUsage(
    )
{
    return --m_UsageCount;
}

__checkReturn HRESULT
CImageBrushContext::GetBitmapSource( 
    __deref_out CBitmapSource** ppSource 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppSource);

    *ppSource = m_Source;
    AddRefObject(m_Source);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CImageBrushContext::SetBitmapSource(
    __in CBitmapSource* pSource
    )
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