#pragma once

#include "GraphicsBrush.h"

class CD2DBrush : public CGraphicsBrush
{
    public:
        virtual ID2D1Brush* GetD2DBrush();

        virtual HRESULT SetTransform( const Matrix3X2& Transform );

    protected:
        CD2DBrush();
        virtual ~CD2DBrush();

        HRESULT Initialize( ID2D1Brush* pBrush );

        ID2D1Brush* m_Brush;
};
