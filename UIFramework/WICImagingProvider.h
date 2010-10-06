#pragma once

#include "ImagingProvider.h"
#include "Factory.h"
#include "WICBitmapSource.h"

#include <Wincodec.h>

typedef HRESULT (WINAPI *SHCreateStreamOnFileWFunc)( LPCWSTR pszFile, DWORD grfMode, IStream** ppstm );
typedef IStream* (WINAPI *SHCreateMemStreamFunc)( const BYTE *pInit, UINT cbInit );

class CWICImagingProvider : public CImagingProvider
{
    public:
        DECLARE_FACTORY( CWICImagingProvider );

        virtual HRESULT LoadBitmapFromFile( const WCHAR* pPath, CBitmapSource** ppBitmapSource );
        virtual HRESULT LoadBitmapFromMemory( const BYTE* pData, UINT32 DataSize, CBitmapSource** ppBitmapSource );

    protected:
        CWICImagingProvider();
        virtual ~CWICImagingProvider();

        HRESULT Initialize();

        HRESULT LoadBitmapFromDecoder( IWICBitmapDecoder* pDecoder, CWICBitmapSource** ppBitmapSource );

        IWICImagingFactory* m_Factory;
        BOOL m_UninitializeCOM;
        HMODULE m_ShlwapiModule;
        SHCreateMemStreamFunc m_SHCreateMemStream;
};
