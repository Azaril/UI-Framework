#pragma once

#include "BitmapSource.h"
#include "Factory.h"

#include <Wincodec.h>

class CWICBitmapSource : public CBitmapSource
{
    public:
        DECLARE_FACTORY1( CWICBitmapSource, IWICBitmapSource* );

        __out IWICBitmapSource* GetWICBitmapSource(
			);

        __override virtual HRESULT GetSize( 
			__out SizeU* pSize 
			);

        __checkReturn HRESULT AssociateStream( 
			__in IStream* pStream 
			);

    protected:
        CWICBitmapSource(
			);

        virtual ~CWICBitmapSource(
			);

        __checkReturn HRESULT Initialize( 
			__in IWICBitmapSource* pSource 
			);

        IWICBitmapSource* m_Source;
        IStream* m_Stream;
};
