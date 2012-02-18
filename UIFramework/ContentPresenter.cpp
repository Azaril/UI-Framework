#include "ContentPresenter.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Properties
//
namespace ContentPresenterProperties
{
    enum Value
    {
        Content
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Content );

//
// Properties
// 
CStaticProperty CContentPresenter::ContentProperty(TypeIndex::ContentPresenter, ContentPresenterProperties::Content, L"Content", TypeIndex::Object, StaticPropertyFlags::Content, &GET_DEFAULT( Content ), &INSTANCE_CHANGE_CALLBACK( CContentPresenter, OnContentChanged ));

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CContentPresenter, OnContentChanged );

CContentPresenter::CContentPresenter() : m_ContentDirty(TRUE),
                                         m_ContentChild(NULL)
{
}

CContentPresenter::~CContentPresenter()
{
    ReleaseObject(m_ContentChild);
}

CUIElement* CContentPresenter::GetTemplateParentForChildren()
{
    return this;
}

HRESULT CContentPresenter::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::OnAttach(Context));

    IFC(EnsureContentUI());

Cleanup:
    return hr;
}

HRESULT CContentPresenter::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::OnDetach(Context));

Cleanup:
    return hr;
}

HRESULT CContentPresenter::SetContent(CObjectWithType* pContent)
{
    HRESULT hr = S_OK;

    if(pContent)
    {
        IFC(SetValue(&CContentPresenter::ContentProperty, pContent));
    }
    else
    {
        IFC(ClearValue(&CContentPresenter::ContentProperty));
    }

Cleanup:
    return hr;
}

HRESULT CContentPresenter::OnContentChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    m_ContentDirty = TRUE;

    IFC(RevokeContentUI());

    IFC(EnsureContentUI());

Cleanup:
    return hr;
}

HRESULT CContentPresenter::RevokeContentUI()
{
    HRESULT hr = S_OK;

    if(m_ContentChild != NULL)
    {
        IFC(RemoveLogicalChild(m_ContentChild));

        ReleaseObject(m_ContentChild);
    }

Cleanup:
    return hr;
}

HRESULT CContentPresenter::EnsureContentUI()
{
    HRESULT hr = S_OK;

    if(m_ContentDirty)
    {
        if(IsAttached())
        {
            IFC(RevokeContentUI());

            IFCEXPECT(m_ContentChild == NULL);

            IFC(GetUIForContent(&m_ContentChild));

            if(m_ContentChild)
            {
                IFC(AddLogicalChild(m_ContentChild));
            }

            m_ContentDirty = FALSE;
        }
    }

Cleanup:
    return hr;
}

HRESULT CContentPresenter::GetUIForContent(CUIElement** ppContentUI)
{
    HRESULT hr = S_OK;
    CObjectWithType* pContent = NULL;
    CTypeValue* pType = NULL;

    IFCPTR(ppContentUI);

    IFC(GetEffectiveValue(&ContentProperty, &pContent));

    if(pContent)
    {
        if(pContent->IsTypeOf(TypeIndex::UIElement))
        {
            *ppContentUI = (CUIElement*)pContent;
            pContent = NULL;
        }
        else
        {
            //TODO: Implement data templates.
            //IFC(CTypeValue::Create(pContent->GetType(), &pType));

            //if(SUCCEEDED(FindResource(pType, &pTemplate)))
            //{
            //}
        }
    }

Cleanup:
    ReleaseObject(pContent);
    ReleaseObject(pType);

    return hr;
}

HRESULT CContentPresenter::GetContentChild(CUIElement** ppChild)
{
    HRESULT hr = S_OK;

    IFCPTR(ppChild);

    IFC(EnsureContentUI());

    *ppChild = m_ContentChild;
    AddRefObject(m_ContentChild);

Cleanup:
    return hr;
}

HRESULT CContentPresenter::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ContentProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation))
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CContentPresenter::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CContentPresenter::ContentProperty)
    {
        *ppLayeredValue = &m_Content;
    }
    else
    {
        hr = CFrameworkElement::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CContentPresenter::MeasureInternal( 
    const SizeF& AvailableSize,
    SizeF& DesiredSize
    )
{
    HRESULT hr = S_OK;
    SizeF ChildSizeDesired;

    if(m_ContentChild != NULL)
    {
        IFC(m_ContentChild->Measure(AvailableSize));

        ChildSizeDesired = m_ContentChild->GetDesiredSize();
    }

    DesiredSize.width = ChildSizeDesired.width;
    DesiredSize.height = ChildSizeDesired.height;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CContentPresenter::ArrangeInternal(
    const SizeF& AvailableSize,
    SizeF& UsedSize 
    )
{
    HRESULT hr = S_OK;

    if(m_ContentChild != NULL)
    {
        IFC(m_ContentChild->Arrange(MakeRect(AvailableSize)));
    }

    UsedSize = AvailableSize;
      
Cleanup:
    return hr;
}

HRESULT CContentPresenter::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    CHitTestResult* pVisualHitTestResult = NULL;

    IFCPTR(ppHitTestResult);

    //IFC(m_BorderVisual->HitTest(LocalPoint, &pVisualHitTestResult));

    if(pVisualHitTestResult)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

    *ppHitTestResult = pVisualHitTestResult;

Cleanup:
    ReleaseObject(pVisualHitTestResult);

    return hr;
}