#pragma once

#include <d3d10.h>

#include "Factory.h"
#include "VertexBuffer.h"

class CD3D10VertexBuffer : public CRefCountedObjectBase< IVertexBuffer >
{
    public:
        DECLARE_FACTORY1( CD3D10VertexBuffer, ID3D10Buffer* );

        __out ID3D10Buffer* GetD3DBuffer(
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
        CD3D10VertexBuffer(
            );

        virtual ~CD3D10VertexBuffer(
            );

        __checkReturn HRESULT Initialize(
            __in ID3D10Buffer* pBuffer
            );

        ID3D10Buffer* m_pBuffer;
        UINT32 m_MaximumVertexCount;
        UINT32 m_StoredVertices;
};

