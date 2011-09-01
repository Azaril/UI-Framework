#pragma once

#include "D2DBrush.h"
#include "Factory.h"

class CD2DBitmapBrush : public CD2DBrush
{
    public:
        DECLARE_FACTORY1( CD2DBitmapBrush, ID2D1BitmapBrush* );

    protected:
        CD2DBitmapBrush(
			);

        virtual ~CD2DBitmapBrush(
			);

        __checkReturn HRESULT Initialize( 
			__in ID2D1BitmapBrush* pBrush 
			);

        ID2D1BitmapBrush* m_BitmapBrush;
};
