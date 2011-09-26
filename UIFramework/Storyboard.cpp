#include "Storyboard.h"
#include "UIElement.h"
#include "Control.h"

//
// Properties
//
CStaticProperty CStoryboard::ChildrenProperty(L"Children", TypeIndex::AnimationTimeline, StaticPropertyFlags::Content | StaticPropertyFlags::Collection | StaticPropertyFlags::ReadOnly);
CStaticProperty CStoryboard::TargetProperty(L"Target", TypeIndex::Object, StaticPropertyFlags::Attached);
CStaticProperty CStoryboard::TargetNameProperty(L"TargetName", TypeIndex::String, StaticPropertyFlags::Attached);
CStaticProperty CStoryboard::TargetPropertyProperty(L"TargetProperty", TypeIndex::String, StaticPropertyFlags::Attached);

CStoryboard::CStoryboard(
    )
    : m_pTimelines(NULL)
{
}

CStoryboard::~CStoryboard(
    )
{
    ReleaseObject(m_pTimelines);
}

__checkReturn HRESULT
CStoryboard::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CAnimationTimelineCollection::Create(&m_pTimelines));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CStoryboard::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ChildrenProperty,
        &TargetProperty,
        &TargetNameProperty,
        &TargetPropertyProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

__override __checkReturn HRESULT 
CStoryboard::SetValueInternal(
    __in CProperty* pProperty,
    __in CObjectWithType* pValue 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CStoryboard::ChildrenProperty)
    {
        IFC(CastType(pValue, &m_pTimelines));

        AddRefObject(m_pTimelines);
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CStoryboard::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CStoryboard::ChildrenProperty)
    {
        *ppValue = m_pTimelines;
        AddRefObject(m_pTimelines);
    }
    else
    {
        IFC(CPropertyObject::GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CStoryboard::ResolveAnimations( 
    __in CUIElement* pElement,
    __in IResolvedAnimationCallback* pCallback 
    )
{
    HRESULT hr = S_OK;
    CClassResolver* pClassResolver = NULL;
    CObjectWithType* pTargetNameValue = NULL;
    CObjectWithType* pTargetPropertyValue = NULL;
    CNamescope* pNamescope = NULL;
    CStringValue* pTargetNameString = NULL;
    CStringValue* pTargetPropertyString = NULL;
    CObjectWithType* pAnimationTarget = NULL;
    CProperty* pTargetProperty = NULL;

    pClassResolver = pElement->GetProviders()->GetClassResolver();
    IFCPTR(pClassResolver);

    if (m_pTimelines != NULL)
    {
        for (UINT32 i = 0; i < m_pTimelines->GetCount(); ++i)
        {
            CAnimationTimeline* pTimeline = m_pTimelines->GetAtIndex(i);

            if (pAnimationTarget == NULL)
            {
                IFC(pTimeline->GetValue(&CStoryboard::TargetProperty, &pAnimationTarget));
            }

            if (pAnimationTarget == NULL)
            {
                IFC(pTimeline->GetValue(&CStoryboard::TargetNameProperty, &pTargetNameValue));

                if (pTargetNameValue != NULL)
                {
                    CControl* pElementAsControl = NULL;

                    IFC(CastType(pTargetNameValue, &pTargetNameString));

                    if (SUCCEEDED(CastType(pElement, &pElementAsControl)))
                    {
                        IFC(pElementAsControl->GetTemplateChild(pTargetNameString->GetValue(), &pAnimationTarget));
                    }

                    if(pAnimationTarget == NULL)
                    {
                        pNamescope = pElement->GetNamescope();
                        IFCPTR(pNamescope);

                        IFC(pNamescope->FindName(pTargetNameString->GetValue(), &pAnimationTarget));
                    }
                }
            }

            if (pAnimationTarget != NULL)
            {
                IFC(pTimeline->GetValue(&CStoryboard::TargetPropertyProperty, &pTargetPropertyValue));

                if (pTargetPropertyValue != NULL)
                {
                    IFC(CastType(pTargetPropertyValue, &pTargetPropertyString));

                    if(SUCCEEDED(pClassResolver->ResolveProperty(pTargetPropertyString->GetValue(), pAnimationTarget->GetType(), &pTargetProperty)))
                    {
                        if (pTargetProperty->GetType() == pTimeline->GetProvidedType())
                        {
                            CAnimatable* pAnimatableTarget = NULL;

                            if(SUCCEEDED(CastType(pAnimationTarget, &pAnimatableTarget)))
                            {
                                IFC(pCallback->AddAnimation(pAnimatableTarget, pTargetProperty, pTimeline));
                            }
                        }
                    }
                }
            }

            ReleaseObject(pTargetNameValue);
            ReleaseObject(pTargetPropertyValue);
            ReleaseObject(pAnimationTarget);
            ReleaseObject(pTargetProperty);
        }
    }

Cleanup:
    ReleaseObject(pTargetNameValue);
    ReleaseObject(pTargetPropertyValue);
    ReleaseObject(pAnimationTarget);
    ReleaseObject(pTargetProperty);

    return hr;
}