#include "TimeController.h"

CTimeController::CTimeController(
    ) 
    : m_CallingSinks(FALSE)
    , m_CurrentSinkIndex(0)
{
}

CTimeController::~CTimeController(
    )
{
}

__checkReturn HRESULT 
CTimeController::Initialize(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CTimeController::UpdateTime(
    const CTimeSpan& TimeDelta
    )
{
    HRESULT hr = S_OK;
    CTimeSink* pCurrentSink = NULL;

    m_CallingSinks = TRUE;

    for(m_CurrentSinkIndex = 0; m_CurrentSinkIndex < m_Sinks.size(); ++m_CurrentSinkIndex)
    {
        SetObject(pCurrentSink, m_Sinks[m_CurrentSinkIndex]);

        IFC(pCurrentSink->OnTimeUpdate(TimeDelta));

        ReleaseObject(pCurrentSink);
    }

Cleanup:
    m_CallingSinks = FALSE;

    ReleaseObject(pCurrentSink);

    return hr;
}

__override __checkReturn HRESULT 
CTimeController::AddSink(
    __in CTimeSink* pSink
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSink);

    //NOTE: Don't validate as this will potentially contain every active animation in the tree.

    m_Sinks.push_back(pSink);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CTimeController::RemoveSink(
    __in CTimeSink* pSink
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSink);

    //NOTE: Don't validate as this will potentially contain every active animation in the tree.

    for (UINT32 i = 0; i < m_Sinks.size(); ++i)
    {
        if (m_Sinks[i] == pSink)
        {
            m_Sinks.erase(m_Sinks.begin() + i);

            if (m_CallingSinks)
            {
                if (i <= m_CurrentSinkIndex)
                {
                    --m_CurrentSinkIndex;
                }
            }
        }
    }

Cleanup:
    return hr;
}