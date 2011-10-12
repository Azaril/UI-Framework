#pragma once

#include "ImagingProvider.h"
#include "Factory.h"

class CCGImagingProvider : public CImagingProvider
{
	public:
		DECLARE_FACTORY( CCGImagingProvider );
    
        __override virtual __checkReturn HRESULT LoadBitmapFromStream(
            __in IReadStream* pStream,
            __deref_out CBitmapSource** ppBitmapSource
            );

	protected:
		CCGImagingProvider(
			);

		virtual ~CCGImagingProvider(
			);

		__checkReturn HRESULT Initialize(
			);
    
        static size_t ProviderGetBytes(
            void* pContext,
            void* pBuffer,
            size_t count
            );
                               
		static off_t ProviderSkipForward(
			void* pContext,
			off_t count
			);
			
		static void ProviderRewind(
			void* pContext
			);

		static void ProviderReleaseInfo(
			void* pContext
			);
};