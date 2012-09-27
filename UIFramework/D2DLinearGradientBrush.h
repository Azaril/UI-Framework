#pragma once

#if defined(FRAMEWORK_D2D)

#include "D2DBrush.h"
#include "Factory.h"

class CD2DLinearGradientBrush : public CD2DBrush
{
    public:
        DECLARE_FACTORY1( CD2DLinearGradientBrush, ID2D1LinearGradientBrush* );

    protected:
        CD2DLinearGradientBrush(
			);

        virtual ~CD2DLinearGradientBrush(
			);

        __checkReturn HRESULT Initialize( 
			__in ID2D1LinearGradientBrush* pBrush 
			);

        ID2D1LinearGradientBrush* m_LinearGradientBrush;
};

#endif