#pragma once

#include "Factory.h"
#include "D2DRenderTarget.h"

class CD2DHWNDRenderTarget : public CD2DRenderTarget
{
    public:
        DECLARE_FACTORY1( CD2DHWNDRenderTarget, ID2D1HwndRenderTarget* );

        ID2D1HwndRenderTarget* GetD2DHWNDRenderTarget();

    protected:
        CD2DHWNDRenderTarget();
        virtual ~CD2DHWNDRenderTarget();

        HRESULT Initialize( ID2D1HwndRenderTarget* pRenderTarget );

        ID2D1HwndRenderTarget* m_HWNDRenderTarget;
};