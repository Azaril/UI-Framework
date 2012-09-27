#pragma once

#if defined(FRAMEWORK_D2D)

#include "Factory.h"
#include "D2DRenderTarget.h"

class UIFRAMEWORK_API CD2DSurfaceRenderTarget : public CD2DRenderTarget
{
    public:
        DECLARE_FACTORY2( CD2DSurfaceRenderTarget, IDXGISurface*, ID2D1RenderTarget* );

    protected:
        CD2DSurfaceRenderTarget(
			);

        virtual ~CD2DSurfaceRenderTarget(
			);

        __checkReturn HRESULT Initialize(
			__in IDXGISurface* pSurface, 
			__in ID2D1RenderTarget* pRenderTarget 
			);

        IDXGISurface* m_Surface;
};

#endif