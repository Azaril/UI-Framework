#include "TimeController.h"

CTimeController::CTimeController(
    ) 
    : m_LastTime(CTime::Now())
    , m_CallingSinks(FALSE)
{
}

CTimeController::~CTimeController(
    )
{
    Disconnect();
}

__checkReturn HRESULT 
CTimeController::Initialize(
    const CTime& Time
    )
{
    HRESULT hr = S_OK;

    m_LastTime = Time;

    return hr;
}

__checkReturn HRESULT 
CTimeController::Disconnect(
    )
{
    HRESULT hr = S_OK;

    for(vector< CTimeSink* >::iterator It = m_Sinks.begin(); It != m_Sinks.end(); ++It)
    {
        (*It)->Release();
    }

    m_Sinks.clear();

    return hr;
}

__checkReturn HRESULT 
CTimeController::UpdateTime(
    const CTime& Time
    )
{
    HRESULT hr = S_OK;
    CTimeSpan TimeDelta = Time - m_LastTime;

    m_LastTime = Time;

    IFC(CallSinks(TimeDelta));

    for(vector< CAsyncOp >::iterator It = m_AsyncOperations.begin(); It != m_AsyncOperations.end(); ++It)
    {
        switch(It->GetOperation())
        {
            case AsyncTimeControllerOp::Add:
                {
                    IFC(AddSinkInternal(It->GetSink()));

                    break;
                }

            case AsyncTimeControllerOp::Remove:
                {
                    IFC(RemoveSinkInternal(It->GetSink()));

                    break;
                }
        }
    }

    m_AsyncOperations.clear();

Cleanup:
    return hr;
}

__checkReturn HRESULT
CTimeController::CallSinks(
    const CTimeSpan& Delta
    )
{
    HRESULT hr = S_OK;

    m_CallingSinks = TRUE;

    for(vector< CTimeSink* >::iterator It = m_Sinks.begin(); It != m_Sinks.end(); ++It)
    {
        IFC((*It)->OnTimeUpdate(Delta));
    }

Cleanup:
    m_CallingSinks = FALSE;

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

    if(!m_CallingSinks)
    {
        IFC(AddSinkInternal(pSink));
    }
    else
    {
        m_AsyncOperations.push_back(CAsyncOp(AsyncTimeControllerOp::Add, pSink));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTimeController::AddSinkInternal(
    __in CTimeSink* pSink
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSink);

    AddRefObject(pSink);

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

    if(!m_CallingSinks)
    {
        IFC(RemoveSinkInternal(pSink));
    }
    else
    {
        m_AsyncOperations.push_back(CAsyncOp(AsyncTimeControllerOp::Remove, pSink));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CTimeController::RemoveSinkInternal(
    __in CTimeSink* pSink
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pSink);

    for(vector< CTimeSink* >::iterator It = m_Sinks.begin(); It != m_Sinks.end(); ++It)
    {
        if((*It) == pSink)
        {
            (*It)->Release();

            m_Sinks.erase(It);
        }
    }

Cleanup:
    return hr;
}