#include "BeginStoryboard.h"
#include "TimeController.h"
#include "UIElement.h"

//
// Properties
//
namespace BeginStoryboardProperties
{
    enum Value
    {
        Storyboard
    };
}

//
// Properties
//
CStaticProperty CBeginStoryboard::StoryboardProperty(TypeIndex::BeginStoryboard, BeginStoryboardProperties::Storyboard, L"Storyboard", TypeIndex::Storyboard, StaticPropertyFlags::Content);

CBeginStoryboard::CBeginStoryboard(
    )
{
}

CBeginStoryboard::~CBeginStoryboard(
    )
{
}

__checkReturn HRESULT 
CBeginStoryboard::Initialize(
    __in CProviders* pProviders 
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT
CBeginStoryboard::CreatePropertyInformation( 
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &StoryboardProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}

__checkReturn HRESULT 
CBeginStoryboard::ResolveAction(
    __in CUIElement* pObject,
    __in IStyleCallback* pCallback,
    __deref_out CResolvedTriggerAction** ppResolvedAction
    )
{
    HRESULT hr = S_OK;
    CResolvedBeginStoryboard* pResolvedBeginStoryboard = NULL;
    CStoryboard* pStoryboard = NULL;

    IFC(CResolvedBeginStoryboard::Create(pObject->GetTimeSource(), &pResolvedBeginStoryboard));

    IFC(GetTypedEffectiveValue(&StoryboardProperty, &pStoryboard));

    if (pStoryboard != NULL)
    {
        IFC(pStoryboard->ResolveAnimations(pObject, pResolvedBeginStoryboard));
    }

    *ppResolvedAction = pResolvedBeginStoryboard;
    pResolvedBeginStoryboard = NULL;

Cleanup:
    ReleaseObject(pResolvedBeginStoryboard);
    ReleaseObject(pStoryboard);

    return hr;
}

__override __checkReturn HRESULT 
CBeginStoryboard::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetOwningType() == TypeIndex::BeginStoryboard)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case BeginStoryboardProperties::Storyboard:
                {
                    *ppLayeredValue = &m_Storyboard;
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
        IFC_NOTRACE(CTriggerAction::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

CResolvedBeginStoryboard::CResolvedBeginStoryboard(  
    )
    : m_pTimeController(NULL)
    , m_pTimeSource(NULL)
    , m_RegisteredToTimeSource(FALSE)
{
}

CResolvedBeginStoryboard::~CResolvedBeginStoryboard(
    )
{
    if (m_pTimeSource != NULL)
    {
        if (m_RegisteredToTimeSource)
        {
            IGNOREHR(m_pTimeSource->RemoveSink(this));
        }

        ReleaseObject(m_pTimeSource);
    }

    ReleaseObject(m_pTimeController);
}

__checkReturn HRESULT
CResolvedBeginStoryboard::Initialize(
    __in CTimeSource* pTimeSource
    )
{
    HRESULT hr = S_OK;

    IFC(CTimeController::Create(&m_pTimeController));

    SetObject(m_pTimeSource, pTimeSource);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CResolvedBeginStoryboard::AddAnimation(
    __in CAnimatable* pTarget,
    __in CProperty* pProperty,
    __in CAnimationTimeline* pTimeline
    )
{
    HRESULT hr = S_OK;

    m_Animations.push_back(CClockInfo(pTarget, pProperty, pTimeline, NULL));

    return hr;
}

__override __checkReturn HRESULT
CResolvedBeginStoryboard::Apply(
    )
{
    HRESULT hr = S_OK;
    CAnimationClock* pAnimationClock = NULL;

    for (vector< CClockInfo >::iterator it = m_Animations.begin(); it != m_Animations.end(); ++it)
    {
        IFC(it->GetTimeline()->CreateClock(m_pTimeController, &pAnimationClock));

        it->SetClock(pAnimationClock);

        IFC(it->GetAnimatable()->ApplyAnimationClock(it->GetProperty(), it->GetClock()));

        ReleaseObject(pAnimationClock);
    }

    if (!m_RegisteredToTimeSource)
    {
        IFC(m_pTimeSource->AddSink(this));

        m_RegisteredToTimeSource = TRUE;
    }

Cleanup:
    ReleaseObject(pAnimationClock);

    return hr;
}

__override __checkReturn HRESULT 
CResolvedBeginStoryboard::OnTimeUpdate( 
    const CTimeSpan& TimeDelta 
    )
{
    HRESULT hr = S_OK;

    IFC(m_pTimeController->UpdateTime(TimeDelta));

Cleanup:
    return hr;
}