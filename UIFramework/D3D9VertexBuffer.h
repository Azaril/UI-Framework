#pragma once

#if defined(FRAMEWORK_D3D9)

#include "Types.h"
#include <d3d9.h>

#include "Factory.h"
#include "VertexBuffer.h"

class CD3D9VertexBuffer : public CRefCountedObjectBase< IVertexBuffer >
{
    public:
        DECLARE_FACTORY1( CD3D9VertexBuffer, IDirect3DVertexBuffer9* );

        __out IDirect3DVertexBuffer9* GetD3DVertexBuffer(
            );

        __override virtual __checkReturn HRESULT SetVertices(
            __in_ecount(VertexCount) RenderVertex* pVertices,
            UINT32 VertexCount            
            );

        __override virtual UINT32 GetVertexCount(
            );

        __override virtual UINT32 GetMaximumVertices(
            );

    protected:
        CD3D9VertexBuffer(
            );

        virtual ~CD3D9VertexBuffer(
            );

        __checkReturn HRESULT Initialize(
            __in IDirect3DVertexBuffer9* pVertexBuffer
            );

        IDirect3DVertexBuffer9* m_pVertexBuffer;
        UINT32 m_MaximumVertexCount;
        UINT32 m_StoredVertices;
};

#endif