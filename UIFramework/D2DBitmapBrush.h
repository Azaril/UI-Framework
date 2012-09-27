#pragma once

#if defined(FRAMEWORK_D2D)

#include "D2DBrush.h"
#include "Factory.h"

class CD2DBitmapBrush : public CD2DBrush
{
    public:
        DECLARE_FACTORY2( CD2DBitmapBrush, ID2D1BitmapBrush*, const SizeU& );

        __override virtual __checkReturn HRESULT GetBounds( 
            RectF& brushBounds
            );

    protected:
        CD2DBitmapBrush(
			);

        virtual ~CD2DBitmapBrush(
			);

        __checkReturn HRESULT Initialize( 
			__in ID2D1BitmapBrush* pBrush,
            const SizeU& size
			);

        ID2D1BitmapBrush* m_BitmapBrush;
        RectF m_Bounds;
};

#endif