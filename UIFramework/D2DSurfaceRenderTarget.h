#pragma once

#include "Factory.h"
#include "D2DRenderTarget.h"

class CD2DSurfaceRenderTarget : public CD2DRenderTarget
{
    public:
        DECLARE_FACTORY2( CD2DSurfaceRenderTarget, IDXGISurface*, ID2D1RenderTarget* );

    protected:
        CD2DSurfaceRenderTarget();
        virtual ~CD2DSurfaceRenderTarget();

        HRESULT Initialize( IDXGISurface* pSurface, ID2D1RenderTarget* pRenderTarget );

        IDXGISurface* m_Surface;
};