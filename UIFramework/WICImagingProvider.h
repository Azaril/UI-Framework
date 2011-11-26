#pragma once

#include "ImagingProvider.h"
#include "Factory.h"
#include "WICBitmapSource.h"

#include <Wincodec.h>

typedef HRESULT (WINAPI *SHCreateStreamOnFileWFunc)(
	LPCWSTR pszFile, 
	DWORD grfMode, 
	IStream** ppstm 
	);

typedef IStream* (WINAPI *SHCreateMemStreamFunc)(
	const BYTE *pInit, 
	UINT cbInit 
	);

class CWICImagingProvider : public CImagingProvider
{
    public:
        DECLARE_FACTORY( CWICImagingProvider );

        __override virtual __checkReturn HRESULT LoadBitmapFromFile(
			__in_z const WCHAR* pPath, 
			__deref_out CBitmapSource** ppBitmapSource 
			);

        __override virtual __checkReturn HRESULT LoadBitmapFromMemory(
			__in_bcount(DataSize) const BYTE* pData, 
			UINT32 DataSize,
			__deref_out CBitmapSource** ppBitmapSource
			);

        __override virtual __checkReturn HRESULT LoadBitmapFromStream(
            __in IReadStream* pStream,
            __deref_out CBitmapSource** ppBitmapSource
            );

    protected:
        CWICImagingProvider(
			);

        virtual ~CWICImagingProvider(
			);

        __checkReturn HRESULT Initialize(
			);

        __checkReturn HRESULT LoadBitmapFromDecoder( 
			__in IWICBitmapDecoder* pDecoder, 
			__deref_out CWICBitmapSource** ppBitmapSource 
			);

        IWICImagingFactory* m_Factory;
        bool m_UninitializeCOM;
        HMODULE m_ShlwapiModule;
        SHCreateMemStreamFunc m_SHCreateMemStream;
};
