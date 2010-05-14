#include "Border.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Background );
DEFINE_GET_DEFAULT( Padding, CRectFValue, D2D1::RectF(0, 0, 0, 0) );
DEFINE_GET_DEFAULT( BorderThickness, CFloatValue, 0 );
DEFINE_GET_DEFAULT_NULL( BorderBrush );
DEFINE_GET_DEFAULT( CornerRadius, CFloatValue, 0 );

//
// Properties
//
CStaticProperty CBorder::BackgroundProperty( L"Background", TypeIndex::Brush, StaticPropertyFlags::None, &GET_DEFAULT(Background), &INSTANCE_CHANGE_CALLBACK( CBorder, OnBackgroundChanged ) );
CStaticProperty CBorder::PaddingProperty( L"Padding", TypeIndex::RectF, StaticPropertyFlags::None, &GET_DEFAULT(Padding), &INSTANCE_CHANGE_CALLBACK( CBorder, OnPaddingChanged ) );
CStaticProperty CBorder::BorderThicknessProperty( L"BorderThickness", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT(BorderThickness), &INSTANCE_CHANGE_CALLBACK( CBorder, OnBorderThicknessChanged ) );
CStaticProperty CBorder::BorderBrushProperty( L"BorderBrush", TypeIndex::Brush, StaticPropertyFlags::None, &GET_DEFAULT(BorderBrush), &INSTANCE_CHANGE_CALLBACK( CBorder, OnBorderBrushChanged ) );
CStaticProperty CBorder::CornerRadiusProperty( L"CornerRadius", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT(CornerRadius), &INSTANCE_CHANGE_CALLBACK( CBorder, OnCornerRadiusChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CBorder, OnBackgroundChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CBorder, OnPaddingChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CBorder, OnBorderThicknessChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CBorder, OnBorderBrushChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CBorder, OnCornerRadiusChanged );

CBorder::CBorder() : m_BorderVisual(NULL),
                     m_GeometryDirty(TRUE),
                     m_Background(this, &CBorder::BackgroundProperty),
                     m_Padding(this, &CBorder::PaddingProperty),
                     m_BorderThickness(this, &CBorder::BorderThicknessProperty),
                     m_BorderBrush(this, &CBorder::BorderBrushProperty),
                     m_CornerRadius(this, &CBorder::CornerRadiusProperty)
{
}

CBorder::~CBorder()
{
    Finalize();
}

HRESULT CBorder::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CDecorator::Initialize(pProviders));

    IFC(CGeometryVisual::Create(&m_BorderVisual));

    IFC(m_BorderVisual->SetStrokeThickness(0));

    IFC(AddChildVisual(m_BorderVisual));

Cleanup:
    return hr;
}

HRESULT CBorder::Finalize()
{
    HRESULT hr = S_OK;

    if(m_BorderVisual)
    {
        IFC(RemoveChildVisual(m_BorderVisual));

        ReleaseObject(m_BorderVisual);
    }

Cleanup:
    return hr;
}

HRESULT CBorder::SetChild(CUIElement* pChild)
{
    HRESULT hr = S_OK;

    IFC(CDecorator::SetChild(pChild));

    IFC(MoveToBack(m_BorderVisual));

Cleanup:
    return hr;
}

HRESULT CBorder::SetBackground(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFC(m_BorderVisual->SetFillBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::SetBorderThickness(FLOAT BorderThickness)
{
    HRESULT hr = S_OK;

    IFCEXPECT(BorderThickness >= 0);

    //m_BorderThickness = BorderThickness;

    //IFC(m_BorderVisual->SetStrokeThickness(BorderThickness));

    //IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CBorder::SetBorder(CBrush* pBrush)
{
    HRESULT hr = S_OK;

    IFC(m_BorderVisual->SetStrokeBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::SetPadding(const RectF& Padding)
{
    HRESULT hr = S_OK;

    IFC(SetPaddingInternal(Padding));

Cleanup:
    return hr;
}

HRESULT CBorder::SetPaddingInternal(const RectF& Padding)
{
    HRESULT hr = S_OK;

    //IFCEXPECT(Padding.left >= 0);
    //IFCEXPECT(Padding.right >= 0);
    //IFCEXPECT(Padding.top >= 0);
    //IFCEXPECT(Padding.bottom >= 0);

    //m_Padding = Padding;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CBorder::SetCornerRadius(FLOAT Radius)
{
    HRESULT hr = S_OK;

    IFC(SetCornerRadiusInternal(Radius));

Cleanup:
    return hr;
}

HRESULT CBorder::SetCornerRadiusInternal(FLOAT Radius)
{
    HRESULT hr = S_OK;

    //IFCEXPECT(Radius >= 0);

    //m_CornerRadius = Radius;

    //IFC(InvalidateGeometry());

Cleanup:
    return hr;
}

HRESULT CBorder::InvalidateGeometry()
{
    HRESULT hr = S_OK;

    m_GeometryDirty = TRUE;

    return hr;
}

HRESULT CBorder::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CDecorator::OnAttach(Context));

    IFC(InvalidateGeometry());

Cleanup:
    return hr;
}

HRESULT CBorder::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(ReleaseGeometry());

    IFC(CDecorator::OnDetach(Context));

Cleanup:
    return hr;
}

HRESULT CBorder::RebuildGeometry()
{
    HRESULT hr = S_OK;
    CRectangleGeometry* pRectangleGeometry = NULL;
    CRoundedRectangleGeometry* pRoundedRectangleGeometry = NULL;
    SizeF FinalSize = GetFinalSize();
    FLOAT BorderThickness = 0;
    FLOAT CornerRadius = 0;
    RectF Rectangle = { 0 };

    IFC(GetEffectiveBorderThickness(&BorderThickness));
    IFC(GetEffectiveCornerRadius(&CornerRadius));

    Rectangle.left = BorderThickness / 2;
    Rectangle.top = BorderThickness / 2;
    Rectangle.right = FinalSize.width - BorderThickness / 2;
    Rectangle.bottom = FinalSize.height - BorderThickness / 2;

    if(CornerRadius == 0)
    {
        IFC(m_VisualContext.GetGraphicsDevice()->CreateRectangleGeometry(Rectangle, &pRectangleGeometry));

        IFC(m_BorderVisual->SetGeometry(pRectangleGeometry));
    }
    else
    {
        IFC(m_VisualContext.GetGraphicsDevice()->CreateRoundedRectangleGeometry(Rectangle, CornerRadius, &pRoundedRectangleGeometry));

        IFC(m_BorderVisual->SetGeometry(pRoundedRectangleGeometry));
    }

    IFC(m_BorderVisual->SetFillBrushTransform(D2D1::Matrix3x2F::Scale(Rectangle.right - Rectangle.left, Rectangle.bottom - Rectangle.top)));

Cleanup:
    ReleaseObject(pRectangleGeometry);
    ReleaseObject(pRoundedRectangleGeometry);

    return hr;
}

HRESULT CBorder::ReleaseGeometry()
{
    HRESULT hr = S_OK;

    IFC(m_BorderVisual->SetGeometry(NULL));

Cleanup:
    return hr;
}

HRESULT CBorder::OnBackgroundChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CBrush* pBrush = NULL;

    IFC(CastType(pNewValue, &pBrush));

    IFC(m_BorderVisual->SetFillBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::OnPaddingChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CBorder::OnBorderThicknessChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CFloatValue* pFloat = NULL;
    
    IFC(InvalidateMeasure());
    IFC(InvalidateGeometry());

    IFC(CastType(pNewValue, &pFloat));

    IFC(m_BorderVisual->SetStrokeThickness((pFloat != NULL) ? pFloat->GetValue() : 0));

Cleanup:
    return hr;
}

HRESULT CBorder::OnBorderBrushChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    CBrush* pBrush = NULL;

    IFC(CastType(pNewValue, &pBrush));

    IFC(m_BorderVisual->SetStrokeBrush(pBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::OnCornerRadiusChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;
    
    IFC(InvalidateGeometry());

Cleanup:
    return hr;
}

HRESULT CBorder::GetEffectiveBackground(CBrush** ppBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(ppBrush);

    IFC(m_Background.GetTypedEffectiveValue(GetProviders(), ppBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::GetEffectivePadding(RectF* pPadding)
{
    HRESULT hr = S_OK;
    CRectFValue* pEffectiveValue = NULL;

    IFC(m_Padding.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pPadding = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CBorder::GetEffectiveBorderThickness(FLOAT* pBorderThickness)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFC(m_BorderThickness.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pBorderThickness = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CBorder::GetEffectiveBorderBrush(CBrush** ppBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(ppBrush);

    IFC(m_BorderBrush.GetTypedEffectiveValue(GetProviders(), ppBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::GetEffectiveCornerRadius(FLOAT* pCornerRadius)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFC(m_CornerRadius.GetTypedEffectiveValue(GetProviders(), &pEffectiveValue));

    *pCornerRadius = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CBorder::PreRenderInternal(CPreRenderContext& Context)
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

HRESULT CBorder::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeF InternalSize = { 0 };
    SizeF BaseSize = { 0 };
    SizeF InternalSizeDesired = { 0 };
    RectF Padding = { 0 };
    FLOAT BorderThickness = 0;
    CUIElement* pChild = NULL;

    IFC(CDecorator::MeasureInternal(AvailableSize, BaseSize));

    IFC(GetEffectiveBorderThickness(&BorderThickness));
    IFC(GetEffectivePadding(&Padding));

    InternalSize.width = max(AvailableSize.width - (BorderThickness * 2) - (Padding.left + Padding.right), 0);
    InternalSize.height = max(AvailableSize.height - (BorderThickness * 2) - (Padding.top + Padding.bottom), 0);

    IFC(GetEffectiveChild(&pChild));

    if(pChild != NULL)
    {
        IFC(pChild->Measure(InternalSize));

        InternalSizeDesired = pChild->GetDesiredSize();
    }

    DesiredSize.width = max(InternalSizeDesired.width + (BorderThickness * 2) + (Padding.left + Padding.right), BaseSize.width);
    DesiredSize.height = max(InternalSizeDesired.height + (BorderThickness * 2) + (Padding.top + Padding.bottom), BaseSize.height);

Cleanup:
    ReleaseObject(pChild);

    return hr;
}

HRESULT CBorder::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;
    CUIElement* pChild = NULL;

    IFC(GetEffectiveChild(&pChild));

    if(pChild != NULL)
    {
        RectF Padding = { 0 };
        FLOAT BorderThickness = 0;

        IFC(GetEffectiveBorderThickness(&BorderThickness));
        IFC(GetEffectivePadding(&Padding));

        SizeF InternalSize = { Size.width - (BorderThickness * 2) - (Padding.left + Padding.right), 
                               Size.height - (BorderThickness * 2) - (Padding.top + Padding.bottom) };

        Matrix3X2 Transform = D2D1::Matrix3x2F::Translation(Padding.left + BorderThickness, Padding.top + BorderThickness);
        
        InternalSize.width = max(InternalSize.width, 0);
        InternalSize.height = max(InternalSize.height, 0);

        IFC(pChild->Arrange(InternalSize));

        IFC(pChild->SetVisualTransform(Transform));
    }

    IFC(CDecorator::ArrangeInternal(Size));

    IFC(InvalidateGeometry());
      
Cleanup:
    ReleaseObject(pChild);

    return hr;
}

HRESULT CBorder::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    CHitTestResult* pVisualHitTestResult = NULL;

    IFCPTR(ppHitTestResult);

    IFC(m_BorderVisual->HitTest(LocalPoint, &pVisualHitTestResult));

    if(pVisualHitTestResult)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

Cleanup:
    ReleaseObject(pVisualHitTestResult);

    return hr;
}

HRESULT CBorder::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    CStaticProperty* Properties[] = 
    {
        &BackgroundProperty,
        &PaddingProperty,
        &BorderThicknessProperty,
        &BorderBrushProperty,
        &CornerRadiusProperty
    };

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CDecorator::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CBorder::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CBorder::BackgroundProperty)
    {
        *ppLayeredValue = &m_Background;
    }
    else if(pProperty == &CBorder::PaddingProperty)
    {
        *ppLayeredValue = &m_Padding;
    }
    else if(pProperty == &CBorder::BorderThicknessProperty)
    {
        *ppLayeredValue = &m_BorderThickness;
    }
    else if(pProperty == &CBorder::BorderBrushProperty)
    {
        *ppLayeredValue = &m_BorderBrush;
    }
    else if(pProperty == &CBorder::CornerRadiusProperty)
    {
        *ppLayeredValue = &m_CornerRadius;
    }
    else
    {
        hr = CDecorator::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}