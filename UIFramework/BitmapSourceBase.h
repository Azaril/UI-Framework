#pragma once

#include "BitmapSource.h"
#include "Texture.h"

class CBitmapSourceBase : public CBitmapSource
{
    public:
        virtual __checkReturn HRESULT LoadIntoTexture(
            __in ITexture* pTexture
            ) = 0;

    protected:
        CBitmapSourceBase(
            );

        virtual ~CBitmapSourceBase(
            );
};

