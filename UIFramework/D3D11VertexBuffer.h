#pragma once

#if defined(FRAMEWORK_D3D11)

#include <d3d11.h>

#include "Factory.h"
#include "VertexBuffer.h"

class CD3D11VertexBuffer : public CRefCountedObjectBase< IVertexBuffer >
{
    public:
        DECLARE_FACTORY2( CD3D11VertexBuffer, ID3D11DeviceContext*, ID3D11Buffer* );

        __out ID3D11Buffer* GetD3DBuffer(
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
        CD3D11VertexBuffer(
            );

        virtual ~CD3D11VertexBuffer(
            );

        __checkReturn HRESULT Initialize(
            __in ID3D11DeviceContext* pContext,
            __in ID3D11Buffer* pBuffer
            );

        ID3D11Buffer* m_pBuffer;
        ID3D11DeviceContext* m_pContext;
        UINT32 m_MaximumVertexCount;
        UINT32 m_StoredVertices;
};

#endif