#include "Border.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CBorder, OnBackgroundChanged );

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
CStaticProperty CBorder::PaddingProperty( L"Padding", TypeIndex::RectF, StaticPropertyFlags::None, &GET_DEFAULT(Padding) );
CStaticProperty CBorder::BorderThicknessProperty( L"BorderThickness", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT(BorderThickness) );
CStaticProperty CBorder::BorderBrushProperty( L"BorderBrush", TypeIndex::Brush, StaticPropertyFlags::None, &GET_DEFAULT(BorderBrush) );
CStaticProperty CBorder::CornerRadiusProperty( L"CornerRadius", TypeIndex::Float, StaticPropertyFlags::None, &GET_DEFAULT(CornerRadius) );

CBorder::CBorder() : m_BorderVisual(NULL),
                     m_GeometryDirty(TRUE),
                     m_Background(&CBorder::BackgroundProperty),
                     m_Padding(&CBorder::PaddingProperty),
                     m_BorderThickness(&CBorder::BorderThicknessProperty),
                     m_BorderBrush(&CBorder::BorderBrushProperty),
                     m_CornerRadius(&CBorder::CornerRadiusProperty)
{
}

CBorder::~CBorder()
{
    Finalize();
}

HRESULT CBorder::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CDecorator::Initialize());

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

    IFC(CFrameworkElement::OnAttach(Context));

    IFC(InvalidateGeometry());

Cleanup:
    return hr;
}

HRESULT CBorder::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(ReleaseGeometry());

    IFC(CFrameworkElement::OnDetach(Context));

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

HRESULT CBorder::OnBackgroundChanged()
{
    HRESULT hr = S_OK;
    CBrush* pBrush = NULL;

    IFC(GetEffectiveBackground(&pBrush));

    IFC(m_BorderVisual->SetFillBrush(pBrush));

Cleanup:
    ReleaseObject(pBrush);

    return hr;
}

HRESULT CBorder::GetEffectiveBackground(CBrush** ppBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(ppBrush);

    IFC(m_Background.GetEffectiveValue(GetProviders(), ppBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::GetEffectivePadding(RectF* pPadding)
{
    HRESULT hr = S_OK;
    CRectFValue* pEffectiveValue = NULL;

    IFC(m_Padding.GetEffectiveValue(GetProviders(), &pEffectiveValue));

    *pPadding = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CBorder::GetEffectiveBorderThickness(FLOAT* pBorderThickness)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFC(m_BorderThickness.GetEffectiveValue(GetProviders(), &pEffectiveValue));

    *pBorderThickness = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CBorder::GetEffectiveBorderBrush(CBrush** ppBrush)
{
    HRESULT hr = S_OK;

    IFCPTR(ppBrush);

    IFC(m_BorderBrush.GetEffectiveValue(GetProviders(), ppBrush));

Cleanup:
    return hr;
}

HRESULT CBorder::GetEffectiveCornerRadius(FLOAT* pCornerRadius)
{
    HRESULT hr = S_OK;
    CFloatValue* pEffectiveValue = NULL;

    IFC(m_BorderThickness.GetEffectiveValue(GetProviders(), &pEffectiveValue));

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

    IFC(CDecorator::MeasureInternal(AvailableSize, BaseSize));

    IFC(GetEffectiveBorderThickness(&BorderThickness));
    IFC(GetEffectivePadding(&Padding));

    InternalSize.width = max(AvailableSize.width - (BorderThickness * 2) - (Padding.left + Padding.right), 0);
    InternalSize.height = max(AvailableSize.height - (BorderThickness * 2) - (Padding.top + Padding.bottom), 0);

    if(m_Child != NULL)
    {
        IFC(m_Child->Measure(InternalSize));

        InternalSizeDesired = m_Child->GetDesiredSize();
    }

    DesiredSize.width = max(InternalSizeDesired.width + (BorderThickness * 2) + (Padding.left + Padding.right), BaseSize.width);
    DesiredSize.height = max(InternalSizeDesired.height + (BorderThickness * 2) + (Padding.top + Padding.bottom), BaseSize.height);

Cleanup:
    return hr;
}

HRESULT CBorder::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;

    if(m_Child != NULL)
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

        IFC(m_Child->Arrange(InternalSize));

        IFC(m_Child->SetVisualTransform(Transform));
    }

    IFC(CDecorator::ArrangeInternal(Size));

    IFC(InvalidateGeometry());
      
Cleanup:
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

HRESULT CBorder::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CBorder::BackgroundProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Brush));

        CBrush* pBrush = (CBrush*)pValue;

        IFC(SetBackground(pBrush));
    }
    else if(pProperty == &CBorder::PaddingProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::RectF));

        CRectFValue* pRectF = (CRectFValue*)pValue;

        IFC(SetPaddingInternal(pRectF->GetValue()))
    }
    else if(pProperty == &CBorder::BorderBrushProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Brush));

        CBrush* pBrush = (CBrush*)pValue;

        IFC(SetBorder(pBrush));
    }
    else if(pProperty == &CBorder::BorderThicknessProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

        CFloatValue* pFloat = (CFloatValue*)pValue;

        IFC(SetBorderThickness(pFloat->GetValue()))
    }
    else if(pProperty == &CBorder::CornerRadiusProperty)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

        CFloatValue* pFloat = (CFloatValue*)pValue;

        IFC(SetCornerRadiusInternal(pFloat->GetValue()))
    }
    else
    {
        IFC(CDecorator::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CBorder::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CBorder::BackgroundProperty)
    {
        IFC(E_NOTIMPL);
    }
    else if(pProperty == &CBorder::PaddingProperty)
    {
        IFC(E_NOTIMPL);
    }
    else if(pProperty == &CBorder::BorderBrushProperty)
    {
        IFC(E_NOTIMPL);
    }
    else if(pProperty == &CBorder::BorderThicknessProperty)
    {
        IFC(E_NOTIMPL);
    }
    else if(pProperty == &CBorder::CornerRadiusProperty)
    {
        IFC(E_NOTIMPL);
    }
    else
    {
        IFC(CDecorator::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}