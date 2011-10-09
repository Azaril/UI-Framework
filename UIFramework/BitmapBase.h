#pragma once

#include "Bitmap.h"
#include "Texture.h"
#include "Factory.h"

class CBitmapBase : public CBitmap
{
    public:
        DECLARE_FACTORY1( CBitmapBase, ITexture* );

        __out ITexture* GetTexture(
            );

    protected:
        CBitmapBase(
            );

        virtual ~CBitmapBase(
            );

        __checkReturn HRESULT Initialize(
            __in ITexture* pTexture
            );

        ITexture* m_pTexture;
};

