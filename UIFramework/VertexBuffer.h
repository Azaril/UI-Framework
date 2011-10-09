#pragma  once

#include "Types.h"
#include "RefCounted.h"
#include "RenderVertex.h"

struct IVertexBuffer
{
    DECLARE_ADDREF_RELEASE_INTERFACE();

    virtual __checkReturn HRESULT SetVertices(
        __in_ecount(VertexCount) RenderVertex* pVertices,
        UINT32 VertexCount            
        ) = 0;

    virtual UINT32 GetVertexCount(
        ) = 0;

    virtual UINT32 GetMaximumVertices(
        ) = 0;
};