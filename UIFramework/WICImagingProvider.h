#pragma once

#include "ImagingProvider.h"
#include "Factory.h"

#include <Wincodec.h>

class CWICImagingProvider : public CImagingProvider
{
    public:
        DECLARE_FACTORY( CWICImagingProvider );

        virtual HRESULT LoadBitmapFromFile( const WCHAR* pPath, CBitmapSource** ppBitmapSource );

    protected:
        CWICImagingProvider();
        virtual ~CWICImagingProvider();

        HRESULT Initialize();

        IWICImagingFactory* m_Factory;
};
