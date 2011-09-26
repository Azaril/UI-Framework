#pragma once

#include "TimeSink.h"
#include "RefCounted.h"

class CTimeSource
{
    public:
        DECLARE_ADDREF_RELEASE_INTERFACE();

        virtual __checkReturn HRESULT AddSink(
            __in CTimeSink* pSink 
            ) = 0;

        virtual __checkReturn HRESULT RemoveSink(
            __in CTimeSink* pSink 
            ) = 0;
};

