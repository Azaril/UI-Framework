#pragma once

#include "BitmapSourceBase.h"
#include "Factory.h"

#include <CoreGraphics/CGImage.h>

class CCGBitmapSource : public CBitmapSourceBase
{
    public:
        DECLARE_FACTORY1( CCGBitmapSource, CGImageRef );
    
        __override virtual __checkReturn HRESULT GetSize(
            __out SizeU* pSize
            );
    
        __override virtual __checkReturn HRESULT LoadIntoTexture(
            __in ITexture* pTexture
            );
    
    protected:
        CCGBitmapSource(
            );
    
        virtual ~CCGBitmapSource(
            );
    
        __checkReturn HRESULT Initialize(
            __in CGImageRef pImage
            );
    
        CGImageRef m_pImage;
};