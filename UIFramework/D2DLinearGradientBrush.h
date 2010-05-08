#pragma once

#include "D2DBrush.h"
#include "Factory.h"

class CD2DLinearGradientBrush : public CD2DBrush
{
    public:
        DECLARE_FACTORY1( CD2DLinearGradientBrush, ID2D1LinearGradientBrush* );

    protected:
        CD2DLinearGradientBrush();
        virtual ~CD2DLinearGradientBrush();

        HRESULT Initialize( ID2D1LinearGradientBrush* pBrush );

        ID2D1LinearGradientBrush* m_LinearGradientBrush;
};
