#pragma once

#include "Types.h"
#include "TimeValue.h"

class CTimeSink
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual HRESULT OnTimeUpdate( const CTimeSpan& TimeDelta ) = 0;
};

