#include "Border.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

CStaticProperty BorderProperties[] = 
{
    CStaticProperty( L"Background", TypeIndex::Brush, StaticPropertyFlags::None ),
    CStaticProperty( L"Padding", TypeIndex::RectF, StaticPropertyFlags::None ),
    CStaticProperty( L"BorderThickness", TypeIndex::Float, StaticPropertyFlags::None ),
    CStaticProperty( L"BorderBrush", TypeIndex::Brush, StaticPropertyFlags::None ),
};

namespace BorderPropertyIndex
{
    enum Value
    {
        Background,
        Padding,
        BorderThickness,
        BorderBrush
    };
}

CBorder::CBorder() : m_BorderVisual(NULL),
                     m_GeometryDirty(TRUE)
{
    m_BorderThickness = 0;

    m_Padding.left = 0;
    m_Padding.top = 0;
    m_Padding.right = 0;
    m_Padding.bottom = 0;
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

    IFC(m_BorderVisual->SetStrokeThickness(m_BorderThickness));

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

    m_BorderThickness = BorderThickness;

    IFC(m_BorderVisual->SetStrokeThickness(BorderThickness));

    IFC(InvalidateMeasure());

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

    IFC(InternalSetPadding(Padding));

Cleanup:
    return hr;
}

HRESULT CBorder::InternalSetPadding(const RectF& Padding)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Padding.left >= 0);
    IFCEXPECT(Padding.right >= 0);
    IFCEXPECT(Padding.top >= 0);
    IFCEXPECT(Padding.bottom >= 0);

    m_Padding = Padding;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CBorder::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::OnAttach(Context));

    m_GeometryDirty = TRUE;

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
    SizeF FinalSize = GetFinalSize();
    RectF Rectangle = { m_BorderThickness / 2, m_BorderThickness / 2, FinalSize.width - m_BorderThickness / 2, FinalSize.height - m_BorderThickness / 2 };

    IFC(m_VisualContext.GetGraphicsDevice()->CreateRectangleGeometry(Rectangle, &pRectangleGeometry));

    IFC(m_BorderVisual->SetGeometry(pRectangleGeometry));

Cleanup:
    ReleaseObject(pRectangleGeometry);

    return hr;
}

HRESULT CBorder::ReleaseGeometry()
{
    HRESULT hr = S_OK;

    IFC(m_BorderVisual->SetGeometry(NULL));

Cleanup:
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

    IFC(CDecorator::MeasureInternal(AvailableSize, BaseSize));

    InternalSize.width = max(AvailableSize.width - (m_BorderThickness * 2) - (m_Padding.left + m_Padding.right), 0);
    InternalSize.height = max(AvailableSize.height - (m_BorderThickness * 2) - (m_Padding.top + m_Padding.bottom), 0);

    if(m_Child != NULL)
    {
        IFC(m_Child->Measure(InternalSize));

        InternalSizeDesired = m_Child->GetDesiredSize();
    }

    DesiredSize.width = max(InternalSizeDesired.width + (m_BorderThickness * 2) + (m_Padding.left + m_Padding.right), BaseSize.width);
    DesiredSize.height = max(InternalSizeDesired.height + (m_BorderThickness * 2) + (m_Padding.top + m_Padding.bottom), BaseSize.height);

Cleanup:
    return hr;
}

HRESULT CBorder::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;

    if(m_Child != NULL)
    {
        SizeF InternalSize = { Size.width - (m_BorderThickness * 2) - (m_Padding.left + m_Padding.right), 
                               Size.height - (m_BorderThickness * 2) - (m_Padding.top + m_Padding.bottom) };

        Matrix3X2 Transform = D2D1::Matrix3x2F::Translation(m_Padding.left + m_BorderThickness, m_Padding.top + m_BorderThickness);
        
        InternalSize.width = max(InternalSize.width, 0);
        InternalSize.height = max(InternalSize.height, 0);

        IFC(m_Child->Arrange(InternalSize));

        IFC(m_Child->SetVisualTransform(Transform));
    }

    IFC(CDecorator::ArrangeInternal(Size));

    m_GeometryDirty = TRUE;
      
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

    IFC(CStaticPropertyInformation::Create(BorderProperties, ARRAYSIZE(BorderProperties), &pStaticInformation))
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

HRESULT CBorder::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= BorderProperties && pProperty < BorderProperties + ARRAYSIZE(BorderProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - BorderProperties);
        
        switch(Index)
        {
            case BorderPropertyIndex::Background:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Brush));

                    CBrush* pBrush = (CBrush*)pValue;

                    IFC(SetBackground(pBrush));

                    break;
                }

            case BorderPropertyIndex::Padding:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::RectF));

                    CRectFValue* pRectF = (CRectFValue*)pValue;

                    IFC(InternalSetPadding(pRectF->GetValue()))

                    break;
                }

            case BorderPropertyIndex::BorderBrush:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Brush));

                    CBrush* pBrush = (CBrush*)pValue;

                    IFC(SetBorder(pBrush));

                    break;
                }

            case BorderPropertyIndex::BorderThickness:
                {
                    IFCEXPECT(pValue->IsTypeOf(TypeIndex::Float));

                    CFloatValue* pFloat = (CFloatValue*)pValue;

                    IFC(SetBorderThickness(pFloat->GetValue()))

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
        IFC(CDecorator::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

HRESULT CBorder::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    // Check if the property is a static property.
    if(pProperty >= BorderProperties && pProperty < BorderProperties + ARRAYSIZE(BorderProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - BorderProperties);
        
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
        IFC(CDecorator::GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}