#pragma once

#if defined(FRAMEWORK_D2D)

#include "Bitmap.h"
#include "Factory.h"

#include <d2d1.h>

class CD2DBitmap : public CBitmap
{
    public:
        DECLARE_FACTORY2( CD2DBitmap, ID2D1Bitmap*, const SizeU& );

        __out ID2D1Bitmap* GetD2DBitmap(
			);

        const SizeU& GetSize(
            );

    protected:
        CD2DBitmap(
			);

        virtual ~CD2DBitmap(
			);

        __checkReturn HRESULT Initialize( 
			__in ID2D1Bitmap* pBitmap,
            const SizeU& size
			);

        ID2D1Bitmap* m_Bitmap;
        SizeU m_Size;
};

#endif