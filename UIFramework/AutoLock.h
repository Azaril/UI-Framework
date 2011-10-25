#pragma once

class CAutoLock
{
    public:
        inline CAutoLock(
            CRITICAL_SECTION* pLock
            )
            : m_pLock(pLock)
        {
            EnterCriticalSection(m_pLock);
        }

        ~CAutoLock(
            )
        {
            LeaveCriticalSection(m_pLock);
        }

    protected:
        CRITICAL_SECTION* m_pLock;
};