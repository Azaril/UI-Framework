#pragma once

#include "Types.h"

class UIFRAMEWORK_API CTimeSpan
{
    public:
        CTimeSpan();
        CTimeSpan( const CTimeSpan& Other );

        static CTimeSpan Zero();
        static CTimeSpan FromMilliseconds( LONGLONG Milliseconds );

        CTimeSpan operator+( const CTimeSpan& Right ) const;
        CTimeSpan& operator+=( const CTimeSpan& Right );
        
        FLOAT operator/( const CTimeSpan& Right ) const;

        bool operator==( const CTimeSpan& Other ) const;

    protected:
        CTimeSpan( LONGLONG Duration );

        LONGLONG m_Duration;
};

class UIFRAMEWORK_API CTime
{
    public:
        CTime( const CTime& Other );

        static CTime Now();

        CTimeSpan operator-( const CTime& Right ) const;

    protected:
        CTime( ULONGLONG Time );

        ULONGLONG m_Time;
};