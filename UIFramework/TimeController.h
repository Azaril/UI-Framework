#pragma once

#include "RefCounted.h"
#include "TimeSource.h"
#include "Collections.h"
#include "Factory.h"
#include "TimeValue.h"

namespace AsyncTimeControllerOp
{
    enum Value
    {
        Add,
        Remove
    };
}

class UIFRAMEWORK_API CTimeController : public CRefCountedObjectBase< CTimeSource >
{
    public:
        DECLARE_FACTORY( CTimeController );

        __checkReturn HRESULT UpdateTime(
            const CTimeSpan& TimeDelta
            );

        __checkReturn HRESULT Disconnect(
            );

        __override virtual __checkReturn HRESULT AddSink( 
            __in CTimeSink* pSink 
            );

        __override virtual __checkReturn HRESULT RemoveSink(
            __in CTimeSink* pSink 
            );

    protected:
        CTimeController(
            );

        virtual ~CTimeController(
            );

        __checkReturn HRESULT Initialize(
            );

        vector< CTimeSink* > m_Sinks;
        bool m_CallingSinks;
        UINT32 m_CurrentSinkIndex;
};

