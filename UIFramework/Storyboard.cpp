#include "Storyboard.h"
#include "UIElement.h"
#include "Control.h"

//
// Properties
//
namespace StoryboardProperties
{
    enum Value
    {
        Children,
        Target,
        TargetName,
        TargetProperty
    };
}

//
// Properties
//
CStaticProperty CStoryboard::ChildrenProperty(TypeIndex::Storyboard, StoryboardProperties::Children, L"Children", TypeIndex::AnimationTimelineCollection, StaticPropertyFlags::Content | StaticPropertyFlags::Collection);
CStaticProperty CStoryboard::TargetProperty(TypeIndex::Storyboard, StoryboardProperties::Target, L"Target", TypeIndex::Object, StaticPropertyFlags::Attached);
CStaticProperty CStoryboard::TargetNameProperty(TypeIndex::Storyboard, StoryboardProperties::TargetName, L"TargetName", TypeIndex::String, StaticPropertyFlags::Attached);
CStaticProperty CStoryboard::TargetPropertyProperty(TypeIndex::Storyboard, StoryboardProperties::TargetProperty, L"TargetProperty", TypeIndex::String, StaticPropertyFlags::Attached);

CStoryboard::CStoryboard(
    )
{
}

CStoryboard::~CStoryboard(
    )
{
}

__checkReturn HRESULT
CStoryboard::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

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
CStoryboard::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::Storyboard)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case StoryboardProperties::Children:
                {
                    *ppLayeredValue = &m_Children;
                    break;
                }

            case StoryboardProperties::Target:
                {
                    *ppLayeredValue = &m_Target;
                    break;
                }

            case StoryboardProperties::TargetName:
                {
                    *ppLayeredValue = &m_TargetName;
                    break;
                }

            case StoryboardProperties::TargetProperty:
                {
                    *ppLayeredValue = &m_TargetProperty;
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
        IFC_NOTRACE(CPropertyObject::GetLayeredValue(pProperty, ppLayeredValue));
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
    CAnimationTimelineCollection* pTimelines = NULL;

    pClassResolver = pElement->GetProviders()->GetClassResolver();
    IFCPTR(pClassResolver);

    IFC(GetTypedEffectiveValue(&ChildrenProperty, &pTimelines));

    if (pTimelines != NULL)
    {
        for (UINT32 i = 0; i < pTimelines->GetCount(); ++i)
        {
            CAnimationTimeline* pTimeline = pTimelines->GetAtIndex(i);

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
    ReleaseObject(pTimelines);

    return hr;
}