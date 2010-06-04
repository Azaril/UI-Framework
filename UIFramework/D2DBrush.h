#pragma once

#include "GraphicsBrush.h"

#include <d2d1.h>

class CD2DBrush : public CGraphicsBrush
{
    public:
        virtual ID2D1Brush* GetD2DBrush();

        virtual HRESULT SetTransform( const Matrix3X2F& Transform );

    protected:
        CD2DBrush();
        virtual ~CD2DBrush();

        HRESULT Initialize( ID2D1Brush* pBrush );

        ID2D1Brush* m_Brush;
};
