#pragma once

#include "D3D9RenderTarget.h"

class UIFRAMEWORK_API CD3D9HWNDRenderTarget : public CD3D9RenderTarget
{
    public:
        DECLARE_FACTORY1( CD3D9HWNDRenderTarget, IDirect3DDevice9* );

        __override virtual SizeF GetSize(
            );

        __override virtual __checkReturn HRESULT BeginRendering(
            );

        __override virtual __checkReturn HRESULT EndRendering(
            );

    protected:
        CD3D9HWNDRenderTarget(
            );

        virtual ~CD3D9HWNDRenderTarget(
            );

        __checkReturn HRESULT Initialize(
            __in IDirect3DDevice9* pDevice
            );

        UINT32 m_Width;
        UINT32 m_Height;
};

