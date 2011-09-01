#pragma once

#include "TimeSink.h"

class CTimeSource
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual __checkReturn HRESULT AddSink(
            __in CTimeSink* pSink 
            ) = 0;

        virtual __checkReturn HRESULT RemoveSink(
            __in CTimeSink* pSink 
            ) = 0;
};

