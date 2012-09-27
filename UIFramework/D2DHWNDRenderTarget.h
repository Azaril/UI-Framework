#pragma once

#if defined(FRAMEWORK_D2D)

#include "Factory.h"
#include "D2DRenderTarget.h"

class UIFRAMEWORK_API CD2DHWNDRenderTarget : public CD2DRenderTarget
{
    public:
        DECLARE_FACTORY1( CD2DHWNDRenderTarget, ID2D1HwndRenderTarget* );

        __out ID2D1HwndRenderTarget* GetD2DHWNDRenderTarget(
			);

    protected:
        CD2DHWNDRenderTarget(
			);

        virtual ~CD2DHWNDRenderTarget(
			);

        __checkReturn HRESULT Initialize( 
			__in ID2D1HwndRenderTarget* pRenderTarget 
			);

        ID2D1HwndRenderTarget* m_HWNDRenderTarget;
};

#endif