#include "TimeValue.h"

CTime::CTime(ULONGLONG Time) : m_Time(Time)
{
}

CTime::CTime(const CTime& Other) : m_Time(Other.m_Time)
{
}

CTime CTime::Now()
{
    LARGE_INTEGER Frequency;
    LARGE_INTEGER Time;

    QueryPerformanceFrequency(&Frequency);

    QueryPerformanceCounter(&Time);

    return CTime((Time.QuadPart * 1000) / Frequency.QuadPart);
}

CTimeSpan CTime::operator-(const CTime& Right) const
{
    return CTimeSpan::FromMilliseconds(m_Time - Right.m_Time);
}

CTimeSpan::CTimeSpan() : m_Duration(0)
{
}

CTimeSpan::CTimeSpan(const CTimeSpan& Other) : m_Duration(Other.m_Duration)
{
}

CTimeSpan::CTimeSpan(LONGLONG Duration) : m_Duration(Duration)
{
}

CTimeSpan CTimeSpan::Zero()
{
    return CTimeSpan(0);
}

CTimeSpan CTimeSpan::FromMilliseconds(LONGLONG Milliseconds)
{
    return CTimeSpan(Milliseconds);
}

CTimeSpan CTimeSpan::operator+(const CTimeSpan& Right) const
{
    return CTimeSpan(m_Duration + Right.m_Duration);
}

FLOAT CTimeSpan::operator/( const CTimeSpan& Right ) const
{
    return ((FLOAT)m_Duration / (FLOAT)Right.m_Duration);
}

CTimeSpan& CTimeSpan::operator+=(const CTimeSpan& Right)
{
    m_Duration += Right.m_Duration;

    return *this;
}

bool CTimeSpan::operator==(const CTimeSpan& Other) const
{
    return (m_Duration == Other.m_Duration);
}

