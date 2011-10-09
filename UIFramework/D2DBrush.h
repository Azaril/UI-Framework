#pragma once

#include "GraphicsBrush.h"

#include <d2d1.h>

class CD2DBrush : public CGraphicsBrush
{
    public:
        virtual __out ID2D1Brush* GetD2DBrush(
			);

        __override virtual __checkReturn HRESULT GetBounds( 
            RectF& brushBounds
            );

        __override virtual __checkReturn HRESULT SetTransform( 
			const Matrix3X2F& Transform 
			);

        __override virtual void GetTransform(
            Matrix3X2F& Transform
            ) const;

    protected:
        CD2DBrush(
			);

        virtual ~CD2DBrush(
			);

        __checkReturn HRESULT Initialize( 
			__in ID2D1Brush* pBrush 
			);

        ID2D1Brush* m_Brush;
};
