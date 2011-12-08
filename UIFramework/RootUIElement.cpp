#include "RootUIElement.h"

CRootUIElement::CRootUIElement(
    ) 
    : m_Child(NULL)
    , m_Namescope(NULL)
{
}

CRootUIElement::~CRootUIElement(
    )
{
    Finalize();
}

__checkReturn HRESULT 
CRootUIElement::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CFrameworkElement::Initialize(pProviders));

    IFC(CNamescope::Create(&m_Namescope));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRootUIElement::SetContext(
    __in CGraphicsDevice* pGraphicsDevice,
    __in CRenderTarget* pRenderTarget, 
    __in CTimeSource* pTimeSource, 
    __in const CFontDescription* pDefaultFont,
    __in CStaticTreeData* pTreeData    
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pGraphicsDevice);
    IFCPTR(pRenderTarget);
    IFCPTR(pTreeData);

    IFCEXPECT(!IsAttached());

    m_FontDescription = *pDefaultFont;

    {
        CVisualAttachContext VisualContext(NULL, NULL, pTimeSource, pGraphicsDevice);

        IFC(OnVisualAttach(VisualContext));
    }

    {
        CUIAttachContext UIContext(pTreeData, NULL, NULL, m_Namescope);

        IFC(OnAttach(UIContext));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CRootUIElement::Finalize(
    )
{
    HRESULT hr = S_OK;

    {
        CUIDetachContext UIContext;

        IFC(OnDetach(UIContext));
    }

    {
        CVisualDetachContext VisualContext(this, NULL, m_VisualContext.GetGraphicsDevice());

        IFC(OnVisualDetach(VisualContext));
    }

    IFC(SetChild(NULL));

Cleanup:
    ReleaseObject(m_Namescope);

    return hr;
}

__checkReturn HRESULT 
CRootUIElement::SetChild(
    __in_opt CUIElement* pChild
    )
{
    HRESULT hr = S_OK;

    if(m_Child)
    {
        IFC(RemoveLogicalChild(m_Child));

        ReleaseObject(m_Child);
    }

    m_Child = pChild;

    if(m_Child)
    {
        AddRefObject(m_Child);

        IFC(AddLogicalChild(m_Child));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CRootUIElement::PreRenderInternal(
    CPreRenderContext& Context
    )
{
    HRESULT hr = S_OK;

    IFC(Context.AddRenderRoot(this));

    IFC(CFrameworkElement::PreRenderInternal(Context));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CRootUIElement::RenderRoot(CRenderContext& Context)
{
    HRESULT hr = S_OK;
    CRenderTarget* pRenderTarget = NULL;

    pRenderTarget = Context.GetRenderTarget();
    IFCPTR(pRenderTarget);

    IFC(pRenderTarget->BeginRendering());

    IFC(Render(Context));

    IFC(pRenderTarget->EndRendering());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CRootUIElement::MeasureInternal(
    SizeF AvailableSize, 
    SizeF& DesiredSize
    )
{
    HRESULT hr = S_OK;

    if(m_Child)
    {
        IFC(m_Child->Measure(AvailableSize));
    }

    DesiredSize = AvailableSize;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CRootUIElement::ArrangeInternal(
    SizeF AvailableSize,
    SizeF& UsedSize
    )
{
    HRESULT hr = S_OK;

    if(m_Child)
    {
        IFC(m_Child->Arrange(MakeRect(AvailableSize)));
    }

    UsedSize = AvailableSize;

Cleanup:
    return hr;
}

__checkReturn HRESULT
CRootUIElement::HitTest(
    Point2F LocalPoint, 
    __deref_out_opt CHitTestResult** ppHitTestResult
    )
{
    HRESULT hr = S_OK;
    SizeF FinalSize = GetFinalSize();

    if(LocalPoint.x >= 0 && LocalPoint.y >= 0 && LocalPoint.x < FinalSize.width && LocalPoint.y < FinalSize.height)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

Cleanup:
    return hr;
}

__out const CFontDescription*
CRootUIElement::GetEffectiveFontDescription(
    )
{
    return &m_FontDescription;
}

//
// CRootUIElement
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value 
CRootUIElement_TypeIndex(
    )
{
    return TypeIndex::RootUIElement;
}

extern "C" UIFRAMEWORK_API
__out CFrameworkElement* 
CRootUIElement_CastTo_CFrameworkElement(
    __in CRootUIElement* pElement
    )
{
    return pElement;
}

extern "C" UIFRAMEWORK_API
__out_opt CRootUIElement* 
CObjectWithType_CastTo_CRootUIElement(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::RootUIElement)) ? (CRootUIElement*)pObject : NULL;
}