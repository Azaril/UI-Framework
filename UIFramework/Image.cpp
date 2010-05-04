#include "Image.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

CStaticProperty ImageProperties[] = 
{
    CStaticProperty( L"Source", TypeIndex::Object, StaticPropertyFlags::None )
};

namespace ImagePropertyIndex
{
    enum Value
    {
        Source
    };
}

CImage::CImage() : m_Source(NULL),
                   m_ImageVisual(NULL),
                   m_ImageBrush(NULL),
                   m_GeometryDirty(TRUE)
{
}

CImage::~CImage()
{
    Finalize();
}

HRESULT CImage::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize());

    IFC(CGeometryVisual::Create(&m_ImageVisual));

    IFC(AddChildVisual(m_ImageVisual));

    IFC(CImageBrush::Create(&m_ImageBrush));

    IFC(m_ImageVisual->SetFillBrush(m_ImageBrush));

Cleanup:
    return hr;
}

HRESULT CImage::Finalize()
{
    HRESULT hr = S_OK;

    if(m_ImageVisual)
    {
        IFC(RemoveChildVisual(m_ImageVisual));

        ReleaseObject(m_ImageVisual);
    }

    ReleaseObject(m_Source);

    ReleaseObject(m_ImageBrush);

Cleanup:
    return hr;
}

HRESULT CImage::SetSource(const WCHAR* pSource)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    IFCPTR(pSource);

    IFC(CStringValue::Create(pSource, &pStringValue));

    IFC(SetSource(pStringValue));

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

HRESULT CImage::SetSource(CObjectWithType* pSource)
{
    HRESULT hr = S_OK;

    IFC(InternalSetSource(pSource));

Cleanup:
    return hr;
}

HRESULT CImage::InternalSetSource(CObjectWithType* pSource)
{
    HRESULT hr = S_OK;

    IFC(m_ImageBrush->SetSource(pSource));

    m_GeometryDirty = TRUE;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CImage::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::OnAttach(Context));

    m_GeometryDirty = TRUE;

Cleanup:
    return hr;
}

HRESULT CImage::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(ReleaseGeometry());

    IFC(CFrameworkElement::OnDetach(Context));

Cleanup:
    return hr;
}

HRESULT CImage::RebuildGeometry()
{
    HRESULT hr = S_OK;
    CRectangleGeometry* pRectangleGeometry = NULL;
    SizeF FinalSize = GetFinalSize();
    RectF Rectangle = { 0, 0, FinalSize.width, FinalSize.height };

    IFC(m_VisualContext.GetGraphicsDevice()->CreateRectangleGeometry(Rectangle, &pRectangleGeometry));

    IFC(m_ImageVisual->SetGeometry(pRectangleGeometry));

Cleanup:
    ReleaseObject(pRectangleGeometry);

    return hr;
}

HRESULT CImage::ReleaseGeometry()
{
    HRESULT hr = S_OK;

    IFC(m_ImageVisual->SetGeometry(NULL));

Cleanup:
    return hr;
}

HRESULT CImage::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    if(m_GeometryDirty)
    {
        m_GeometryDirty = FALSE;

        IFC(RebuildGeometry());
    }

    IFC(CFrameworkElement::PreRenderInternal(Context));

Cleanup:
    return hr;
}

HRESULT CImage::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeU ImageSize = { 0 };
    SizeF BaseSize = { 0 };

    IFC(CFrameworkElement::MeasureInternal(AvailableSize, BaseSize));

    if(m_ImageBrush != NULL)
    {
        IFC(m_ImageBrush->GetSize(&ImageSize));
    }

    if(m_Size.width == 0 && m_Size.height == 0)
    {
        DesiredSize.width = max((FLOAT)ImageSize.width, BaseSize.width);
        DesiredSize.height = max((FLOAT)ImageSize.height, BaseSize.height);
    }
    else if(m_Size.width == 0 && m_Size.height != 0 && ImageSize.height != 0)
    {
        FLOAT Ratio = m_Size.height / ImageSize.height;

        DesiredSize.width = ImageSize.width * Ratio;
        DesiredSize.height = m_Size.height;
    }
    else if(m_Size.width != 0 && m_Size.height == 0 && ImageSize.width != 0)
    {
        FLOAT Ratio = m_Size.width / ImageSize.width;

        DesiredSize.width = m_Size.width;
        DesiredSize.height = ImageSize.height * Ratio;
    }
    else
    {
        DesiredSize.width = BaseSize.width;
        DesiredSize.height = BaseSize.height;
    }

Cleanup:
    return hr;
}

HRESULT CImage::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::ArrangeInternal(Size));

    m_GeometryDirty = TRUE;

Cleanup:
    return hr;
}

HRESULT CImage::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(ImageProperties, ARRAYSIZE(ImageProperties), &pStaticInformation));
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CImage::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= ImageProperties && pProperty < ImageProperties + ARRAYSIZE(ImageProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - ImageProperties);
        
        switch(Index)
        {
            case ImagePropertyIndex::Source:
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
        IFC(CFrameworkElement::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CImage::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    // Check if the property is a static property.
    if(pProperty >= ImageProperties && pProperty < ImageProperties + ARRAYSIZE(ImageProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - ImageProperties);
        
        switch(Index)
        {
            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CFrameworkElement::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

HRESULT CImage::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    CHitTestResult* pVisualHitTestResult = NULL;

    IFCPTR(ppHitTestResult);

    IFC(m_ImageVisual->HitTest(LocalPoint, &pVisualHitTestResult));

    if(pVisualHitTestResult)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

Cleanup:
    ReleaseObject(pVisualHitTestResult);

    return hr;
}