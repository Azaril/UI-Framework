#pragma once

#include "Bitmap.h"
#include "Factory.h"

#include <d2d1.h>

class CD2DBitmap : public CBitmap
{
    public:
        DECLARE_FACTORY1( CD2DBitmap, ID2D1Bitmap* );

        ID2D1Bitmap* GetD2DBitmap();

    protected:
        CD2DBitmap();
        virtual ~CD2DBitmap();

        HRESULT Initialize( ID2D1Bitmap* pBitmap );

        ID2D1Bitmap* m_Bitmap;
};
