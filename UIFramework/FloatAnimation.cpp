#include "FloatAnimation.h"

CFloatAnimation::CFloatAnimation() : m_From(NULL),
                                     m_To(NULL)
{
}

CFloatAnimation::~CFloatAnimation()
{
    ReleaseObject(m_From);
    ReleaseObject(m_To);
}

HRESULT CFloatAnimation::Initialize(FLOAT From, FLOAT To, const CTimeSpan& Duration)
{
    HRESULT hr = S_OK;

    IFC(CFloatValue::Create(From, &m_From));
    IFC(CFloatValue::Create(To, &m_To));
    IFC(CDurationValue::Create(Duration, &m_Duration));

Cleanup:
    return hr;
}

HRESULT CFloatAnimation::GetCurrentValue(CAnimationClock* pClock, CObjectWithType* pDefaultInitialValue, CObjectWithType* pDefaultFinalValue, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CFloatValue* pDefaultFrom = NULL;
    CFloatValue* pDefaultTo = NULL;
    FLOAT Progress = 0;
    CFloatValue* pOutVal = NULL;

    CFloatValue* pFrom = NULL;
    CFloatValue* pTo = NULL;

    IFC(CastType(pDefaultInitialValue, &pDefaultFrom));
    IFC(CastType(pDefaultFinalValue, &pDefaultTo));

    pFrom = (m_From != NULL) ? m_From : pDefaultFrom;
    pTo = (m_To != NULL) ? m_To : pDefaultTo;

    IFCPTR(pFrom);
    IFCPTR(pTo);

    FLOAT FromVal = pFrom->GetValue();
    FLOAT ToVal = pTo->GetValue();

    IFC(pClock->GetCurrentProgress(&Progress));

    FLOAT Range = ToVal - FromVal;

    FLOAT Value = FromVal + (Range * Progress);

    IFC(CFloatValue::Create(Value, &pOutVal));

    *ppValue = pOutVal;
    pOutVal = NULL;

Cleanup:
    ReleaseObject(pOutVal);

    return hr;
}