#include "RefCounted.h"
#include "AssertUtil.h"

#ifdef FRAMEWORK_DEBUG

#ifdef _WIN32
#else

#include <libkern/OSAtomic.h>

#endif

struct TrackableBlock : public TrackableInformation
{
    TrackableBlock* Previous;
    TrackableBlock* Next;
};

TrackableBlock* g_HeadBlock = NULL;
TrackableBlock* g_TailBlock = NULL;
UINT32 g_AllocationID = 0;

void ValidateTrackableBlock(
    __in TrackableBlock* pTrackableBlock
    )
{
    for (TrackableBlock* pBlock = pTrackableBlock; pBlock != NULL; pBlock = pBlock->Next)
    {
        if (pBlock->Next == NULL)
        {
            ASSERT(pBlock == g_TailBlock);
        }
        else
        {
            ASSERT(pBlock->Next->Previous == pBlock);
        }
    }

    for (TrackableBlock* pBlock = pTrackableBlock; pBlock != NULL; pBlock = pBlock->Previous)
    {
        if (pBlock->Previous == NULL)
        {
            ASSERT(pBlock == g_HeadBlock);
        }
        else
        {
            ASSERT(pBlock->Previous->Next == pBlock);
        }
    }
}

__out TrackableInformation* GetNewTrackableInformation(
    )
{
    TrackableBlock* pBlock = new TrackableBlock();
    ASSERT(pBlock);

#ifdef _WIN32
    pBlock->AllocationID = InterlockedIncrement(&g_AllocationID);
#else
    pBlock->AllocationID = OSAtomicIncrement32((INT32*)&g_AllocationID) + 1;
#endif

    if (g_TailBlock == NULL)
    {
        ASSERT(g_HeadBlock == NULL);

        g_HeadBlock = pBlock;
        g_TailBlock = pBlock;

        pBlock->Previous = NULL;
        pBlock->Next = NULL;
    }
    else
    {
        ASSERT(g_HeadBlock != NULL);

        g_TailBlock->Next = pBlock;

        pBlock->Previous = g_TailBlock;
        pBlock->Next = NULL;

        g_TailBlock = pBlock;
    }

    ValidateTrackableBlock(pBlock);

    return pBlock;
}

void FreeTrackableInformation(
    __in_opt TrackableInformation* pTrackableInformation
    )
{
    if (pTrackableInformation != NULL)
    {
        TrackableBlock* pBlock = (TrackableBlock*)pTrackableInformation;

        ValidateTrackableBlock(pBlock);

        if (pBlock->Previous != NULL)
        {
            pBlock->Previous->Next = pBlock->Next;
        }
        else
        {
            g_HeadBlock = pBlock->Next;
        }

        if (pBlock->Next != NULL)
        {
            pBlock->Next->Previous = pBlock->Previous;
        }
        else
        {
            g_TailBlock = pBlock->Previous;
        }

        delete pBlock;
    }
}

void EnumerateTrackableInformation(
    __in ITrackableInformationCallback* pCallback
    )
{
    for (TrackableBlock* pBlock = g_HeadBlock; pBlock != NULL; pBlock = pBlock->Next)
    {
        if (!pBlock->Object->IgnoreTrackableForLeaks())
        {
            pCallback->ProcessInformation(pBlock);
        }
    }
}

#endif