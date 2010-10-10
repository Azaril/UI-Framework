#pragma once

#include "TimeSink.h"

class CTimeSource
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual HRESULT AddSink( CTimeSink* pSink ) = 0;
        virtual HRESULT RemoveSink( CTimeSink* pSink ) = 0;
};

