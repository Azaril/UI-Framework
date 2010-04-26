#pragma once

#include "D2DBrush.h"
#include "Factory.h"

class CD2DSolidColorBrush : public CD2DBrush
{
    public:
        DECLARE_FACTORY1( CD2DSolidColorBrush, ID2D1SolidColorBrush* );

    protected:
        CD2DSolidColorBrush();
        virtual ~CD2DSolidColorBrush();

        HRESULT Initialize( ID2D1SolidColorBrush* pBrush );

        ID2D1SolidColorBrush* m_SolidBrush;
};
