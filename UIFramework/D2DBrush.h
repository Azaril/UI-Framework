#pragma once

#include "Brush.h"

class CD2DBrush : public CBrush
{
    public:
        virtual ID2D1Brush* GetD2DBrush();

    protected:
        CD2DBrush();
        virtual ~CD2DBrush();

        virtual HRESULT Initialize( ID2D1Brush* pBrush );

        ID2D1Brush* m_Brush;
};
