#pragma once

#include "Types.h"
#include "TimeValue.h"
#include "RefCounted.h"

class CTimeSink
{
    public:
        DECLARE_ADDREF_RELEASE_INTERFACE();

        virtual __checkReturn HRESULT OnTimeUpdate( 
            const CTimeSpan& TimeDelta 
            ) = 0;
};

