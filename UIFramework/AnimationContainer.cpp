#include "AnimationContainer.h"

CAnimationContainer::CAnimationContainer() : m_TargetObject(NULL),
                                             m_TimeSource(NULL)
{
}

CAnimationContainer::~CAnimationContainer()
{
    ReleaseObject(m_TimeSource);
}

HRESULT CAnimationContainer::Initialize(IAnimatable* pTargetObject, ITimeSource* pTimeSource)
{
    HRESULT hr = S_OK;

    IFCPTR(pTargetObject);
    IFCPTR(pTimeSource);

    //NOTE: Don't take a strong reference as the object may own the container.
    m_TargetObject = pTargetObject;

    m_TimeSource = pTimeSource;
    AddRefObject(m_TimeSource);

Cleanup:
    return hr;
}

HRESULT CAnimationContainer::BeginAnimation(IAnimation* pAnimation)
{
    HRESULT hr = S_OK;
    UINT32 CurrentTime = 0;

    IFCPTR(pAnimation);

    IFC(m_TimeSource->GetTime(&CurrentTime));

    m_Animations.push_back(AnimationInfo(pAnimation, CurrentTime));

Cleanup:
    return hr;
}

HRESULT CAnimationContainer::OnTimeUpdate(UINT32 Time)
{
    HRESULT hr = S_OK;

    for(std::vector< AnimationInfo >::iterator It = m_Animations.begin(); It != m_Animations.end(); ++It)
    {

    }

Cleanup:
    return hr;
}