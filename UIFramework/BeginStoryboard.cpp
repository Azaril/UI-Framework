#include "BeginStoryboard.h"
#include "TimeController.h"
#include "UIElement.h"

//
// Properties
//
CStaticProperty CBeginStoryboard::StoryboardProperty(L"Storyboard", TypeIndex::Storyboard, StaticPropertyFlags::Content);

CBeginStoryboard::CBeginStoryboard(
    )
    : m_pStoryboard(NULL)
{
}

CBeginStoryboard::~CBeginStoryboard(
    )
{
    ReleaseObject(m_pStoryboard);
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

    IFC(CResolvedBeginStoryboard::Create(pObject->GetTimeSource(), &pResolvedBeginStoryboard));

    if (m_pStoryboard != NULL)
    {
        IFC(m_pStoryboard->ResolveAnimations(pObject, pResolvedBeginStoryboard));
    }

    *ppResolvedAction = pResolvedBeginStoryboard;
    pResolvedBeginStoryboard = NULL;

Cleanup:
    ReleaseObject(pResolvedBeginStoryboard);

    return hr;
}

__override __checkReturn HRESULT 
CBeginStoryboard::SetValueInternal(
    __in CProperty* pProperty,
    __in CObjectWithType* pValue 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty == &CBeginStoryboard::StoryboardProperty)
    {
        IFC(CastType(pValue, &m_pStoryboard));

        AddRefObject(m_pStoryboard);
    }
    else
    {
        IFC(CPropertyObject::SetValueInternal(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CBeginStoryboard::GetValueInternal(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue 
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty == &CBeginStoryboard::StoryboardProperty)
    {
        *ppValue = m_pStoryboard;
        AddRefObject(m_pStoryboard);
    }
    else
    {
        IFC(CPropertyObject::GetValueInternal(pProperty, ppValue));
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