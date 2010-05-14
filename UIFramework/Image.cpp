#include "Image.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Source );

//
// Properties
//
CStaticProperty CImage::SourceProperty( L"Source", TypeIndex::Object, StaticPropertyFlags::None, &GET_DEFAULT( Source ), &INSTANCE_CHANGE_CALLBACK( CImage, OnSourceChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CImage, OnSourceChanged );

CImage::CImage() : m_Source(this, &CImage::SourceProperty),
                   m_ImageVisual(NULL),
                   m_ImageBrush(NULL),
                   m_GeometryDirty(TRUE)
{
}

CImage::~CImage()
{
    Finalize();
}

HRESULT CImage::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize(pProviders));

    IFC(CGeometryVisual::Create(&m_ImageVisual));

    IFC(AddChildVisual(m_ImageVisual));

    IFC(CImageBrush::Create(pProviders, &m_ImageBrush));

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

    IFC(SetValue(&CImage::SourceProperty, pStringValue));

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

HRESULT CImage::SetSource(CObjectWithType* pSource)
{
    HRESULT hr = S_OK;

    IFC(SetValue(&CImage::SourceProperty, pSource));

Cleanup:
    return hr;
}

HRESULT CImage::OnSourceChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(m_ImageBrush->SetSource(pNewValue));

    m_GeometryDirty = TRUE;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CImage::GetEffectiveSource(CObjectWithType** ppSource)
{
    HRESULT hr = S_OK;

    IFCPTR(ppSource);

    IFC(m_Source.GetEffectiveValue(GetProviders(), ppSource));

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

    //TODO: Set brush transform.

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
    FLOAT Width = 0;
    FLOAT Height = 0;

    IFC(GetEffectiveWidth(&Width));
    IFC(GetEffectiveHeight(&Height));

    IFC(CFrameworkElement::MeasureInternal(AvailableSize, BaseSize));

    if(m_ImageBrush != NULL)
    {
        IFC(m_ImageBrush->GetSize(&ImageSize));
    }

    if(Width == 0 && Height == 0)
    {
        DesiredSize.width = max((FLOAT)ImageSize.width, BaseSize.width);
        DesiredSize.height = max((FLOAT)ImageSize.height, BaseSize.height);
    }
    else if(Width == 0 && Height != 0 && ImageSize.height != 0)
    {
        FLOAT Ratio = Height / ImageSize.height;

        DesiredSize.width = ImageSize.width * Ratio;
        DesiredSize.height = Height;
    }
    else if(Width != 0 && Height == 0 && ImageSize.width != 0)
    {
        FLOAT Ratio = Width / ImageSize.width;

        DesiredSize.width = Width;
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

    CStaticProperty* Properties[] = 
    {
        &SourceProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
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

HRESULT CImage::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CImage::SourceProperty)
    {
        *ppLayeredValue = &m_Source;
    }
    else
    {
        hr = CFrameworkElement::GetLayeredValue(pProperty, ppLayeredValue);
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