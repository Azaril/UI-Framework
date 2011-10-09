#pragma once

#include "BitmapSource.h"
#include "Factory.h"
#include "BitmapSourceBase.h"

#include <Wincodec.h>

class CWICBitmapSource : public CBitmapSourceBase
{
    public:
        DECLARE_FACTORY2( CWICBitmapSource, IWICImagingFactory*, IWICBitmapSource* );

        __out IWICBitmapSource* GetWICBitmapSource(
			);

        __override virtual HRESULT GetSize( 
			__out SizeU* pSize 
			);

        __checkReturn HRESULT AssociateStream( 
			__in IStream* pStream 
			);

        __override virtual __checkReturn HRESULT LoadIntoTexture(
            __in ITexture* pTexture
            );

        __checkReturn HRESULT GetSourceAsFormat(
            const WICPixelFormatGUID& format,
            IWICBitmapSource** ppSource
            );

    protected:
        CWICBitmapSource(
			);

        virtual ~CWICBitmapSource(
			);

        __checkReturn HRESULT Initialize( 
            __in IWICImagingFactory* pFactory,
			__in IWICBitmapSource* pSource 
			);

        IWICImagingFactory* m_pFactory;
        IWICBitmapSource* m_pSource;
        IStream* m_pStream;
};
